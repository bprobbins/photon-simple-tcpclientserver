//useful for testing server
"use strict";

var net = require('net');

var HOST = '192.168.1.9'; //241'; //9';

var PORT = 6123;

var loopCounter = 0


var replymsg = 'TheInMsg and then a whole lot more characters than before';
var clientmsg = 'mymsg 2 and then a whole lot more characters than before';

//    var client = new net.Socket();

var recursive = function () {

   var client = new net.Socket();

    client.connect(PORT, HOST, function () {


client.write(clientmsg);

// Add a 'data' event handler for the client socket
// data is what the server sent to this socket
client.on('data', function (data) {

    console.log('Response received: ' + data);

/*
    if (data == replymsg) {
            // Write the data back to the socket, the client will receive it as data from the server
            sock.write(clientmsg);
    }
*/

    // Close the client socket completely
  client.destroy();
//client.write(clientmsg);
});


      client.on('error', function (err)  {
        console.log('Socket error: ');
        console.log(err.stack);
      });

        console.log('CONNECTED TO: ' + HOST + ':' + PORT);
        // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client
//        client.write('x');
//client.write(clientmsg);
/*
        // Add a 'data' event handler for the client socket
        // data is what the server sent to this socket
        client.on('data', function (data) {

            console.log('Response received: ' + data);

            if (data == replymsg) {
                    // Write the data back to the socket, the client will receive it as data from the server
                    sock.write(clientmsg);
            }


            // Close the client socket completely
          client.destroy();
//client.write(clientmsg);
        });
*/

    // Add a 'close' event handler for the client socket
    client.on('close', function () {
            console.log('Connection closed');
            loopCounter++;
            console.log('Loop #: ' + loopCounter)
        });

  //        client.destroy();


        setTimeout(recursive, 150);

    })

}

recursive();
