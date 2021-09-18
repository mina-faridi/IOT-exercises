from coapthon.server.coap import CoAP

from coapthon.resources.resource import Resource

class DataResource(Resource):
    def __init__(self, name="DataResource", coap_server=None):
        super(DataResource, self).__init__(name, coap_server, visible=True, observable=True, allow_children=True)
        self.payload = "0"

    def render_GET(self, request):
        return self

    def render_PUT_advanced(self, request, response):
        self.payload = request.payload
        print(request.payload)
        if request.payload == "D9 38 13 C3":
            response.payload = "0"
        elif request.payload == "14 C8 66 2B":
            response.payload = "1"
        return response
   

class CoAPServer(CoAP):
    def __init__(self, host, port):
        CoAP.__init__(self, (host, port))
        self.add_resource('path', DataResource())


def main():
    server = CoAPServer("192.168.1.106", 5683)
    try:
        server.listen(10)
    except KeyboardInterrupt:
        print("Server Shutdown")
        server.close()
        print("Exiting...")


if __name__ == '__main__':
    main()
