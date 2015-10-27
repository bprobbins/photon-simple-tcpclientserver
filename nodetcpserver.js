//useful to test your client

var net = require('net');

var HOST = 'XXX.XXX.X.XXX'; //insert your computer's ip address
var PORT = 6123;

// Create a server instance, and chain the listen function to it
// The function passed to net.createServer() becomes the event handler for the 'connection' event
// The sock object the callback function receives UNIQUE for each connection
net.createServer(function(sock) {
    
    // We have a connection - a socket object is assigned to the connection automatically
    console.log('CONNECTED: ' + sock.remoteAddress +':'+ sock.remotePort);
    
    // Add a 'data' event handler to this instance of socket
    sock.on('data', function(data) {
        
        console.log('DATA ' + sock.remoteAddress + ': ' + data);
        // Write the data back to the socket, the client will receive it as data from the server
        sock.write('9');
        
    });
 
   
    sock.on('error', function (error) {
        console.log('******* ERROR ' + error + ' *******');

        // close connection
 //       sock.end();
    });
    

    // Add a 'close' event handler to this instance of socket
    sock.on('close', function(data) {
        console.log('CLOSED: ************************');// + sock.remoteAddress +' '+ sock.remotePort);
    });


    
}).listen(PORT, HOST);

console.log('Server listening on ' + HOST +':'+ PORT);