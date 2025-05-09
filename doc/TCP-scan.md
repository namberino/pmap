# Full TCP Hanshake Scan

The full TCP handshake scan is found in [scan.h](../include/scan.h). This scan takes in a host string and a port number and perform a TCP handshake to the specified host and port.

This method of scanning is very primitive and very slow. SYN scan and parallel scanning is necessary to improve performance.
