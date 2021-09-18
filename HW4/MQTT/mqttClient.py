import paho.mqtt.client as mqtt

#define the function
def on_message(node, userdata, message):
	data = message.payload.decode("utf-8")
	print("data is: ")
	print(data)
	if data == "D9 38 13 C3":
		print("tag detected")
		node.publish("responseMQTT", 0)
	elif data == "14 C8 66 2B":
		print("card detected")
		node.publish("responseMQTT", 1)
	print("message publish to no")


def on_connect(client, userdata, flags, rc):
	print("connected with result code {0}".format(str(rc)))
	client.subscribe("requestMQTT")


#assign the functions
node_2 = mqtt.Client("node_2")
node_2.on_connect = on_connect
node_2.on_message = on_message
node_2.connect("192.168.1.102", 1883)


node_2.loop_forever()