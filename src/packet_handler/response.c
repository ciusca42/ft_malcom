#include "../../includes/ft_malcom.h" 


int send_packet(reply_struct reply) {
    
    ssize_t bytes_sent;

    bytes_sent = sendto(reply.sockfd, 
            reply.buffer,
            reply.buff_size,
            0,
            reply.addr,
            reply.addr_len);
    
    return bytes_sent;
}
