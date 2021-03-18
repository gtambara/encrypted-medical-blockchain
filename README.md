# medical-encrypted-blockchain
This project was idealized to serve as a small-scale model for a data storage system based on the strucure proposed by Satoshi Nakamoto. The project, a considerable part of the design of a complete Blockchain, proved to be efficient in what it proposes to do, and able to store much more safely than usual, making it ideal for use in systems that require some care with the content to be saved. This works with a model not very complex and easy to understand, intended to be used for learning purposes. The SHA-256 hash function is able to guarantee that the encrypted hash is not decodable, if implemented in a network with its due alterations, it's capable of being easily verifiable for cases where data has changed during insertion or by error. 

To test it with the gcc compiler, it goes as follows on the terminal:

>gcc -o exec main.c auxiliar.c -lssl -lcrypto
