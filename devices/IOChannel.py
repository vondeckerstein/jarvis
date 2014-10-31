class IOChannel:
	def __init__(self):
		self.attr={}
		self.attr['type']='Generic IO'
	def getAttr(self, key = None):
		if key is None:
			return self.attr
		else:
			return self.attr[key]
	def setAttr(self, key, value):
		self.attr[key]=value
	def toString(self):
		print str(self.attr['id'])
		message = 'IO #' + str(self.attr['id']) + ' - ' + self.attr['type'] + " - '" + self.attr['name']
		message += "' on link " + self.link.getAttr('id') + ' at pin/channel ' + str(self.attr['pin'])
		print message
		return 	message	