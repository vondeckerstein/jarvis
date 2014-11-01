import pdb
import sys, time
import datetime
from flask import Flask, render_template, request
import configLoader
import colors
app = Flask(__name__)

@app.route("/")
def main():
	templateData = {
	'message' : jarvis.attr['message'],
	}
	return render_template('main.html', **templateData)
	
@app.route('/status', methods=['POST'])
def manualSend():
	device = request.form['id']
	command = request.form['text']
	return status(command, device)
	
@app.route("/status")
def status(command=None, link=0):
	if command !=None:
		command = command.replace("$","#")
		response = jarvis.links[int(link)].ask(str(command))
	else:
		command = ''
		response = ''
	timeString = datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
	templateData = {
		'time': timeString,
		'jarvis' : jarvis
	}
	return render_template('status.html', **templateData)
	
@app.route('/print', methods=['POST'])
def printRequest():
	print "loops: " + str(powerprint.powerprint(request))
	return "success"
	
@app.route('/command', methods=['POST'])
def sendCommand():
	if 'caller' in request.form:
		print request.form.keys
		caller = request.form['caller']
		if request.form.has_key(caller):
			value = request.form[caller] 
		else:
			value = 'off'
		channel = int(''.join(char for char in caller if char.isdigit()))
		if value.isdigit():
			value=int(value)
		print [caller, value, channel]
		jarvis.channels[channel].write(value)
	else:
		print "bad request:"
		print str(request)
	jarvis.sync()
	return command()
	
@app.route("/command")
def command(message=None): 
	channels=[]
	for channel in jarvis.channels:
		channels.append(generateChannelDisplay(channel,'/command'))
	templateData = {
		'message' : message,
		'jarvis' : jarvis,
		'channels' : channels
		}
	return render_template('command.html', **templateData)
	
def generateChannelDisplay(channel, url):
	try:
		templateData = {
			'url': url,
			'channel' : channel
			}
		return render_template( 'elements/' + channel.attr['type'] + '.html', **templateData)
	except Exception, e:
		print [e, channel.attr['type']]
		return "Channel template error"
	
if __name__ == "__main__":
	jarvis  = configLoader.ConfigLoader().getConfiguredHost()
	print jarvis.getAttr('message')
	print "serving..."
	app.run(host='0.0.0.0', port=80, debug=True)
	print "app finished!"
	
	
	

		
		
	
	
	
	
	
	
	
	
	
	
	