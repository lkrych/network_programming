// Dumps raw memory in hex byte and printable split format
// it is used to display packet data by the server program
void dump (const unsigned char *data_buffer, const unsigned int length) {
    unsigned char byte;
    unsigned int i, j;
    for (i = 0; i < length; i++) {
        byte = data_buffer[i];
        printf("%02x ", data_buffer[i]); //display byte in hex
        if (((i%16) == 15) || (i == length -1)) {
            for (j = 0; j < 15 - (i % 16); j++) {
                printf(" ");
            }
            printf("| ");
            for (j=(i-(i%16)); j <= i; j++) { // Display printable bytes from line
                byte = data_buffer[j];
                if ((byte > 31) && (byte < 127)) { //Outside printable char range
                    printf("%c", byte);
                }
                else {
                    printf(".");
                }
            }
            printf("\n"); //End of the dump line (each line is 16 bytes)
        }
    }
}

// you may notice that every packet that is received ends with
//  the bytes OxOD and OxOA. This is how telnet terminates the lines
// It sends a carraige return and a newline character. The HTTP protocol
// also expects these two bytes

//server output

// Server: got connection from 127.0.0.1 port 58454
// RECV: 2 bytes
// 0d 0a               | ..
// RECV: 22 bytes
// 48 65 6c 6c 6f 20 53 69 6d 70 6c 65 20 53 65 72 | Hello Simple Ser
// 76 65 72 21 0d 0a           | ver!..

//telnet output

// telnet 127.0.0.1 7890 <- Simple server
// Trying 127.0.0.1...
// Connected to localhost.
// Escape character is '^]'.
// Hello, world!
// Hello Simple Server!
// ^C^C^]
// telnet> quit
// Connection closed.