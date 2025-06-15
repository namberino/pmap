# Custom timeout for scan

In the `tcp_scan` function, we can't directly set a timeout for `connect()`. What we need to do is use non-blocking mode for sockets.

Blocking mode (synchronous mode) is the default mode for sockets. A blocking call doesn't return to the program until the event that was requested has been completed.

Non-blocking (asynchronous mode) mode makes it so that any calls would return to the program immediately to reveal whether the requested service was completed, even if they can't be completed immediately. The program can continue to operate without having to wait for the task to finish. This allows us to implement our own timeout rules for the socket.

Timeout needs to be set in miliseconds. The function first grab the status flag of the client file descriptor and add non-blocking bits to the flag. In non-blocking mode, `connect()` returns `errno` as `EINPROGRESS`.

The file descriptor will become writable once the connection is successful or failed. `select()` is used for monitoring the status of the file descriptor. `getsockopt()` is used to check whether the connection was successful (open port) or not (closed port). 

After the scan, the socket is returned to blocking mode.
