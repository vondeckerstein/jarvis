import time
import datetime
import threading
import configLoader


class JarvisHost:
	def __init__(self):
		self.channels=[]
		self.links=[]
		self.rules=[]
		self.attr = {}
		self.attr['uptime']=datetime.datetime.now()
		self.attr['message']='Jarvis Command Protocol initiated.'
		self.attr['counter']=0
		self.attr['syncInterval'] =10
		self.run()
	
	def __del__(self):
		self.running = False
		try:
			self.syncProcess.join(self.attr['syncInterval']+1)
		except:
			print "could not rejoin sync process!"
	def run(self):
		print "starting thread!"
		self.running = True
		self.syncProcess = threading.Thread(target = self.__run, name = 'jarvisSyncProcess')
		self.syncProcess.daemon = True
		self.syncProcess.start()
		print "thread started!"
		
	def __run(self):
		while self.running:
			time.sleep(self.attr['syncInterval'])
			self.sync()
	def sync(self):
		print str(datetime.datetime.now()) + ": syncing: " + str(self)
		for counter, rule in enumerate(self.rules):
			try:
				rule.run()
			except KeyboardInterrupt:
				raise
			except Exception, e:
				print "could not run rule #" + counter
		for channel in self.channels:
			channel.read()
		self.attr['counter']=self.attr['counter']+1
	def getAttr(self, key=None):
		if key is None:
			return self.attr
		else:
			return self.attr[key]
	def addChannel(self, channel):
		self.channels.append(channel)
	def addLink(self, link):
		self.links.append(link)
	def readChannel(self, id):
		try:
			return self.Channels[id].read()
		except:
			print 'Read error, ch ' + id
			return 'Could not read!'
	def writeChannel(self, id, message):
		print 'writing ' + str(id) + ':' + str(message)
		self.channels[id].write(message)
	def toString(self):
		message = ["Jarvis Host v.7"]
		for key in self.attr:
			message.append(key + ": " + str(self.attr[key]))
		for link in self.links:
			message.append(link.toString())
		for channel in self.channels:
			message.append(channel.toString())
		for rule in self.rules:
			message.append(rule.toString())
		return message
	def getChannels(self):
		return len(self.channels)
	def getChannelAttr(self, ch, attr = None):
		return self.channels[ch].getAttr(attr)
	def getLinkAttr(self, link, attr = None):
		return self.links[link].getAttr(attr)
	def setChannelAttr(self, ch, attr, value):
		return channels[ch].setAttr(attr, value)
	def getLinks(self):
		return len(self.links)
	def manualSend(self, link, message):
		message = self.links[int(link)].ask(str(message))
		return message
	def getRules(self):
		return len(self.rules)
	def getRuleAttr(self, rule, attr = None):
		return self.rules[rule].getAttr(attr)
	def setRuleAttr(self, rule, attr, value):
		return rule[rule].setAttr(attr, value)
	def addRule(self, rule):
		self.rules.append(rule)