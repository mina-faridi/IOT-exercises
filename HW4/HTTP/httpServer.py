from flask import Flask, request

app = Flask(__name__)


@app.route('/submitValue')
def submitValueHandler():
    global value
    value = request.args.get('value')
    print("value is " + value)
    return 'The value is: {}'.format(value)


@app.route('/value')
def valueHandler():
    return value


@app.route('/submitCommand')
def submitCommandHandler():
    global command
    command = request.args.get("command")
    print("command is " + command)
    return 'The command is: {}'.format(command)


@app.route('/command')
def commandHandler():
    return command


value = ""
command = ""
app.run(host='192.168.1.102', port=8090)
