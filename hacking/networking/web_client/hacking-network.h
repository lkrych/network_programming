void fatal(char *); //a function for fatal errors see below

/* This function accepts a socket FD and a prt to the null terminated string to send.
* The function will make make sure all the bytes of the string are sent.
* Returns 1 on success and 0 on failure
*/
int send_string(int sockfd, unsigned char *buffer) {
    int sent_bytes, bytes_to_send;
    bytes_to_send = strlen(buffer);
    while(bytes_to_send > 0) {
        sent_bytes = send(sockfd, buffer, bytes_to_send, 0);
        if (sent_bytes == -1) {
            return 0; // Return 0 on send serror
        }
        bytes_to_send -= sent_bytes;
        buffer += sent_bytes;
    }
    return 1; //return 1 on successs
}

/* This function accepts a socket FD and a ptr to a destination buffer.
* It will receive from the socket until the EOL byte sequence is seen.
* The EOL bytes are read from the socket, bu tthe destination buffer is 
* terminated before these bytes. Returns the size of the read line (w/o EOL bytes)
*/
int recv_line(int sockfd, unsigned char *dest_buffer) {
#define EOL "\r\n" // EOL byte sequence
#define EOL_SIZE 2
    unsigned char *ptr;
    int eol_matched = 0;

    ptr = dest_buffer;
    while(recv(sockfd, ptr, 1, 0) == 1) { // read a single byte
        if (*ptr == EOL[eol_matched]) { // does this byte match terminator?
            eol_matched++;
            if (eol_matched == EOL_SIZE) { // if all bytes match terminator,
                *(ptr+1-EOL_SIZE) = '\0'; // terminate the string
                return strlen(dest_buffer); // return bytes received
            }
        } else {
            eol_matched = 0;
        }
        ptr++; //Increment the pointer to the next byte
    }
    return 0; // Didn't find EOL
    
}

// a function to display an error message and then exit
void fatal(char *message) {
    char error_message[100];

    strcpy(error_message, "[!!] Fatal Error ");
    strncat(error_message, message, 83);
    perror(error_message);
    exit(-1);
}