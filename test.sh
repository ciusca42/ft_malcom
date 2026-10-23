#!/usr/bin/env bash
#
# Strict argument-parsing test suite for a program expecting:
#   ./prog <ip> <mac> <ip> <mac>
#
# ASSUMPTIONS (change these to match your actual program's contract):
#   - exit code 0  => arguments were accepted / parsed successfully
#   - exit code !=0 => arguments were rejected
#   - MAC must be lowercase hex, colon-separated: xx:xx:xx:xx:xx:xx
#   - IPv4 only, standard dotted-quad, no leading zeros, 0-255 per octet
#   - order is strictly ip, mac, ip, mac (swapping types must fail)
#
# Usage: ./test_args.sh /path/to/your/binary
set -u

BIN="${1:-}"
if [[ -z "$BIN" || ! -x "$BIN" ]]; then
    echo "Usage: $0 /path/to/compiled/binary"
    exit 2
fi

PASS=0
FAIL=0
FAILED_NAMES=()

# run_test <name> <expected: valid|invalid> arg1 arg2 ...
run_test() {
    local name="$1"; shift
    local expect="$1"; shift
    local out rc

    out=$("$BIN" "$@" 2>&1)
    rc=$?

    if [[ "$expect" == "valid" ]]; then
        if [[ $rc -eq 0 ]]; then
            PASS=$((PASS+1))
        else
            FAIL=$((FAIL+1)); FAILED_NAMES+=("$name (expected ACCEPT, got rc=$rc, out=\"$out\")")
        fi
    else
        if [[ $rc -ne 0 ]]; then
            PASS=$((PASS+1))
        else
            FAIL=$((FAIL+1)); FAILED_NAMES+=("$name (expected REJECT, got rc=0, out=\"$out\")")
        fi
    fi
}

# run_test_argc <name> <expect> -- exact argv count control (handles 0 args, empty strings, etc.)
run_test_argc() {
    run_test "$@"
}

VALID_IP1="192.168.1.1"
VALID_MAC1="aa:bb:cc:dd:ee:ff"
VALID_IP2="10.0.0.254"
VALID_MAC2="00:11:22:33:44:55"

echo "==================================================================="
echo " 1. BASELINE VALID CASES"
echo "==================================================================="
run_test "baseline valid"                          valid   "$VALID_IP1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "valid, min IP 0.0.0.0"                    valid   "0.0.0.0" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "valid, max IP 255.255.255.255"            valid   "255.255.255.255" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "valid, MAC all zeros"                     valid   "$VALID_IP1" "00:00:00:00:00:00" "$VALID_IP2" "$VALID_MAC2"
run_test "valid, MAC broadcast ff:ff:ff:ff:ff:ff"   valid   "$VALID_IP1" "ff:ff:ff:ff:ff:ff" "$VALID_IP2" "$VALID_MAC2"
run_test "valid, both IPs identical"                valid   "$VALID_IP1" "$VALID_MAC1" "$VALID_IP1" "$VALID_MAC2"
run_test "valid, both MACs identical"               valid   "$VALID_IP1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC1"
run_test "valid, all four identical pairs"          valid   "$VALID_IP1" "$VALID_MAC1" "$VALID_IP1" "$VALID_MAC1"

echo "==================================================================="
echo " 2. ARGUMENT COUNT"
echo "==================================================================="
run_test_argc "zero args"                           invalid
run_test_argc "one arg"                             invalid "$VALID_IP1"
run_test_argc "two args"                            invalid "$VALID_IP1" "$VALID_MAC1"
run_test_argc "three args"                          invalid "$VALID_IP1" "$VALID_MAC1" "$VALID_IP2"
run_test_argc "five args (one extra)"               invalid "$VALID_IP1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2" "extra"
run_test_argc "six args"                            invalid "$VALID_IP1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2" "extra1" "extra2"
run_test_argc "four empty-string args"              invalid "" "" "" ""

echo "==================================================================="
echo " 3. ORDER / TYPE SWAPPING (must fail: order is strictly ip,mac,ip,mac)"
echo "==================================================================="
run_test "mac,ip,mac,ip (fully reversed order)"     invalid "$VALID_MAC1" "$VALID_IP1" "$VALID_MAC2" "$VALID_IP2"
run_test "ip,ip,mac,mac (grouped by type)"          invalid "$VALID_IP1" "$VALID_IP2" "$VALID_MAC1" "$VALID_MAC2"
run_test "mac,mac,ip,ip"                            invalid "$VALID_MAC1" "$VALID_MAC2" "$VALID_IP1" "$VALID_IP2"
run_test "ip,mac,mac,ip (swap last two)"            invalid "$VALID_IP1" "$VALID_MAC1" "$VALID_MAC2" "$VALID_IP2"
run_test "mac,ip,ip,mac (swap first two)"           invalid "$VALID_MAC1" "$VALID_IP1" "$VALID_IP2" "$VALID_MAC2"

echo "==================================================================="
echo " 4. IPv4 — NUMERIC RANGE / FORMAT"
echo "==================================================================="
run_test "octet 256 (out of range)"                 invalid "192.168.1.256" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "octet 999"                                invalid "192.168.1.999" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "negative octet"                           invalid "192.168.1.-1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "only 3 octets"                             invalid "192.168.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "only 2 octets"                             invalid "192.168" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "only 1 octet"                              invalid "192" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "5 octets"                                  invalid "192.168.1.1.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "leading zero octet 01"                     invalid "192.168.01.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "leading zeros all octets"                  invalid "192.168.001.001" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "octet 00 (ambiguous octal-ish)"            invalid "192.168.00.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "leading plus sign"                         invalid "192.168.1.+1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "trailing dot"                              invalid "192.168.1.1." "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "leading dot"                                invalid ".192.168.1.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "double dot"                                 invalid "192..168.1.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "hex-like octet 0x1"                         invalid "192.168.1.0x1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "non-numeric octet"                          invalid "192.168.1.abc" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "empty octet"                                invalid "192.168..1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IP with embedded space"                     invalid "192.168.1. 1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IP with leading space"                      invalid " 192.168.1.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IP with trailing space"                     invalid "192.168.1.1 " "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IP with tab"                                 invalid $'192.168.1.1\t' "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IP with newline"                             invalid $'192.168.1.1\n' "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IP with CIDR suffix"                         invalid "192.168.1.1/24" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IP with port suffix"                         invalid "192.168.1.1:80" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IPv6 address"                                invalid "::1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IPv6-mapped IPv4"                            invalid "::ffff:192.168.1.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "hostname instead of IP"                      invalid "localhost" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "empty string as IP"                          invalid "" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "just dots"                                   invalid "..." "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "decimal-as-single-int IP (2130706433)"       invalid "2130706433" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "fullwidth unicode digits"                    invalid "１９２.168.1.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "IP argument is a MAC"                        invalid "$VALID_MAC1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"

echo "==================================================================="
echo " 5. MAC — FORMAT"
echo "==================================================================="
run_test "uppercase hex MAC"                           invalid "$VALID_IP1" "AA:BB:CC:DD:EE:FF" "$VALID_IP2" "$VALID_MAC2"
run_test "mixed-case hex MAC"                          invalid "$VALID_IP1" "Aa:bB:cC:dD:eE:fF" "$VALID_IP2" "$VALID_MAC2"
run_test "dash-separated MAC"                          invalid "$VALID_IP1" "aa-bb-cc-dd-ee-ff" "$VALID_IP2" "$VALID_MAC2"
run_test "dot-separated MAC (Cisco style)"             invalid "$VALID_IP1" "aabb.ccdd.eeff" "$VALID_IP2" "$VALID_MAC2"
run_test "no-separator MAC"                            invalid "$VALID_IP1" "aabbccddeeff" "$VALID_IP2" "$VALID_MAC2"
run_test "space-separated MAC"                         invalid "$VALID_IP1" "aa bb cc dd ee ff" "$VALID_IP2" "$VALID_MAC2"
run_test "underscore-separated MAC"                    invalid "$VALID_IP1" "aa_bb_cc_dd_ee_ff" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with only 5 groups"                      invalid "$VALID_IP1" "aa:bb:cc:dd:ee" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with 7 groups"                           invalid "$VALID_IP1" "aa:bb:cc:dd:ee:ff:11" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with 1-char group"                       invalid "$VALID_IP1" "a:bb:cc:dd:ee:ff" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with 3-char group"                       invalid "$VALID_IP1" "aaa:bb:cc:dd:ee:ff" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with non-hex char (g)"                   invalid "$VALID_IP1" "gg:bb:cc:dd:ee:ff" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with trailing colon"                     invalid "$VALID_IP1" "aa:bb:cc:dd:ee:ff:" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with leading colon"                      invalid "$VALID_IP1" ":aa:bb:cc:dd:ee:ff" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with double colon"                       invalid "$VALID_IP1" "aa::bb:cc:dd:ee:ff" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC empty string"                            invalid "$VALID_IP1" "" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with embedded space"                     invalid "$VALID_IP1" "aa:bb:cc:dd:ee: ff" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC argument is an IP"                       invalid "$VALID_IP1" "$VALID_IP1" "$VALID_IP2" "$VALID_MAC2"
run_test "MAC with 0x prefix per byte"                 invalid "$VALID_IP1" "0xaa:0xbb:0xcc:0xdd:0xee:0xff" "$VALID_IP2" "$VALID_MAC2"
run_test "decimal-dotted MAC (170.187.204.221.238.255)" invalid "$VALID_IP1" "170.187.204.221.238.255" "$VALID_IP2" "$VALID_MAC2"

echo "==================================================================="
echo " 6. INJECTION / MEMORY-SAFETY STRESS (must not crash; must reject)"
echo "==================================================================="
LONG_STR=$(printf 'a%.0s' {1..10000})
run_test "10000-char IP arg (overflow probe)"          invalid "$LONG_STR" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "10000-char MAC arg (overflow probe)"         invalid "$VALID_IP1" "$LONG_STR" "$VALID_IP2" "$VALID_MAC2"
run_test "format string in IP (%s%s%s%s)"              invalid "%s%s%s%s" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "format string in MAC (%n)"                   invalid "$VALID_IP1" "%n%n%n%n" "$VALID_IP2" "$VALID_MAC2"
run_test "shell metacharacters in IP"                  invalid '$(rm -rf /tmp/x)' "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "shell metacharacters in MAC"                  invalid "$VALID_IP1" '`id`' "$VALID_IP2" "$VALID_MAC2"
run_test "null-byte-containing arg"                     invalid "$(printf '192.168.1.1\x00extra')" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "backslash-heavy arg"                          invalid '\\\\\\\\' "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "leading dash (looks like a flag)"             invalid "-192.168.1.1" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "arg is literally --help"                      invalid "--help" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"
run_test "arg is literally -h"                          invalid "-h" "$VALID_MAC1" "$VALID_IP2" "$VALID_MAC2"

# check the binary didn't crash (segfault/abort) on any of the above by checking $? ranges
echo
echo "Note: check dmesg / run under valgrind separately to confirm none of the"
echo "'stress' cases above actually crashed (segfault exit codes are >128,"
echo "e.g. 139 = SIGSEGV, 134 = SIGABRT) rather than cleanly returning an error."
echo

echo "==================================================================="
echo " RESULTS: $PASS passed, $FAIL failed"
echo "==================================================================="
if [[ $FAIL -gt 0 ]]; then
    echo
    echo "Failed cases:"
    for n in "${FAILED_NAMES[@]}"; do
        echo "  - $n"
    done
fi

exit $(( FAIL > 0 ? 1 : 0 ))