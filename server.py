import socketserver
import datetime
import os
import sys

class MyWebServer(socketserver.BaseRequestHandler):

    status = {200: "HTTP/1.1 200 OK\r\n", 404:"HTTP/1.1 404 Page Not Found\r\n", 405:"HTTP/1.1 405 Method Not Allowed\r\n", 301:"HTTP/1.1 301 Page Moved\r\n"}
    hostname = "127.0.0.1:8000"
    folder = "build-Skittles-Qt_5_15_1_WebAssembly-Debug"
    absPath = os.path.abspath(folder+"/")
    
    
    def handle(self):
        self.data = self.request.recv(4096).strip().decode()
        print("Received a request of: %s\n" % self.data.split('\n')[0].split('\r')[0])
        parse_result = self.parse(self.data)
        response = self.get(parse_result)
        self.request.sendall(response)
        
    def parse(self, request):
        
        verb = request.split('\n')[0].split()
        #verb = parts[0].split()

        if (verb == []):
            return 0

        if (verb[0] != 'GET'):
            # 405, verb not supported
            return 405
            
        return verb[1]  # path
        
    def get(self, statusCode):    
         
        if (statusCode == 0):
            print("Returned:")
            print("-----------------")
            print()
            return bytearray('','utf-8')
        
        # headers
        server = "Server: " + MyWebServer.hostname + "\r\n"
        date = "Date: " + str(datetime.datetime.now()) + "\r\n"
        #conType = "Content-Type: text/html\r\n"
        conLen = "Content-Length: " + '0' + "\r\n"
        connection = "Connection: keep-alive\r\n"
        #location = "Location: " + ""
        end = "\r\n"
        body = bytearray('','utf-8')
        statusMessage = ''
        
        if ((statusCode == 405) or (statusCode == 404)):
            statusMessage = MyWebServer.status[statusCode]
            conType = "Content-Type: text/html\r\n"
            body = open('HTTPresponse/405.html','rb').read() if (statusCode == 405) else open('404.html','r').read()
            conLen = "Content-Length: " + str(len(body)) + "\r\n"
            response_line = ''.join([statusMessage, server, date, conType, conLen, connection, end, body])
                
        else:
            if (('.' not in statusCode) and (statusCode[-1] != '/')):
                # redirect to for a slash
                statusCode += '/'
                statusMessage = MyWebServer.status[301]
                location = "Location: http://" + MyWebServer.hostname + statusCode + "\r\n"
                conType = "Content-Type: text/html\r\n"
                body = open('HTTPresponse/301.html','rb').read()
                conLen = "Content-Length: " + str(len(body)) + "\r\n"
                response_line = ''.join([statusMessage, server, date, conType, conLen, connection, location, end])
            
            else:
                try:
                    # open file to send
                    self.verify('/'+MyWebServer.folder+statusCode)
                    toOpen = (MyWebServer.absPath + statusCode).replace('\\','/')
                    body = open(toOpen,'rb').read()
                    print("Sending file: " + toOpen)
                    statusMessage = MyWebServer.status[200]
                    conLen = "Content-Length: " + str(len(body)) + "\r\n"
                    if (".js" in statusCode):
                        conType = "Content-Type: text/javascript\r\n"
                    elif (".wasm" in statusCode):
                        conType = "Content-Type: application/wasm\r\n"
                    else:
                        conType = "Content-Type: text/" + statusCode.split('.')[-1] + "\r\n"
                except:
                    print("Cannot send file: " + toOpen)
                    statusMessage = MyWebServer.status[404]
                    conType = "Content-Type: text/html\r\n"
                    body = open('HTTPresponse/404.html','rb').read()
                    conLen = "Content-Length: " + str(len(body)) + "\r\n"

                response_line = ''.join([statusMessage, server, date, conType, conLen, connection, end])

            
        response_line = bytearray(response_line,'utf-8') + body
        print("Returned:",statusMessage)  # what was requested
        print("-----------------")
        #print(response_line) # what we return
        
        return response_line
        
    def verify(self, path):
        # verify that the requested path is in current directoty
        path = path[1:]
        requestedPath = os.path.abspath(path)
        
        if (MyWebServer.absPath not in requestedPath):
            raise Exception()

    
if __name__ == "__main__":
    
    HOST = "localhost"
    try:
        PORT = int(sys.argv[1])
    except:
        PORT = 8000

    MyWebServer.hostname = "127.0.0.1:" + str(PORT)

    socketserver.TCPServer.allow_reuse_address = True
    # Create the server, binding to localhost on PORT
    server = socketserver.TCPServer((HOST, PORT), MyWebServer)
    print()
    print("Starting server...")
    print("Listening on:", "localhost:"+str(PORT))
    print("Navigate to", "localhost:"+str(PORT)+"/Skittles.html", "to play chess")
    print("Press Ctrl-C to terminate the server")
    print()
    print("-----------------")

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()
    server.shutdown()
    server.server_close()
    
