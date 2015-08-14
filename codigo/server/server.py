import SocketServer
import cv2
import signal
import sys

cap = cv2.VideoCapture(0)

class ImageRequestHandler(SocketServer.BaseRequestHandler):

    def handle(self):
        print "atendiendo request"
        self.request.setblocking(True)
        while True:
            self.request.recv(10)
            print "sirviendo frame"
            # Capture frame-by-frame
            ret, frame = cap.read()
            data = frame #cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            # Our operations on the frame come here
            self.request.sendall(data)
        return

if __name__ == '__main__':
    import socket
    import threading
    address = ('', 1080) # let the kernel give us a port
    server = SocketServer.TCPServer(address, ImageRequestHandler)
    ip, port = server.server_address # find out what port we were given
    print ip, port
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        server.shutdown()

# When everything done, release the capture
cap.release()
