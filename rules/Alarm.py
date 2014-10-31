from __future__ import division
import datetime
import time
from Rule import *

		
class Alarm(Rule):
	SEND_TIME=60 #seconds
	"""
	time can be passed as a datetime.time object, or as 
	hour= and minute=
	date must be a datetime.date object or it will default
	to 'everyday'
	"""
	def __init__(self, **kwargs):  #todo: cleanup dictionary creation
		self.attr= { 'time':datetime.time(6,0),'date':'everyday','fadeOverMinutes':0,
		'channel':None, 'offMessage':0,'onMessage':100, 'sendOffMessage':False, 
		'outputFunction':None, 'outputFunctionArgs':None, 'hour':None, 'minute':None, 
		'minute':'0' }
		for key in kwargs:
			if key not in self.attr:
				print "unrecognized key: " + key
			self.attr[key]=kwargs[key]
		if self.attr['hour']!=None:
			print 'resetting time'
			self.attr['time']=datetime.time(self.attr['hour'],self.attr['minute'])

		
	def run(self):
		seconds=self.secondsUntilGoesOff()
		self.attr['output']=self.__calculateOutput(seconds)
		
		if self.attr['outputFunction'] and self.attr['output']:
			print "output: " + str(self.attr['output'])
			args=[]
			for arg in self.attr['outputFunctionArgs']:
				args.append(self.attr[arg])
			self.attr['outputFunction'](*args)
			
	def __calculateOutput(self, seconds):
		if self.attr['fadeOverMinutes']==None:
			if seconds<self.SEND_TIME:
				return self.attr['onMessage']
			else:
				if self.attr['sendOffMessage']:
					return self.attr['offMessage']
		elif seconds<self.attr['fadeOverMinutes']*60:
			fadePercentage = 1.0-seconds/self.attr['fadeOverMinutes']/60.0
			totalFade = self.attr['onMessage']-self.attr['offMessage']
			output = self.attr['offMessage'] + totalFade * fadePercentage
			return int(output)
			
				
		pass
	def secondsUntilGoesOff(self):
		now = datetime.datetime.now()
		if self.attr['date']=='everyday':
			alarmDate = datetime.datetime.now().date()
			alarm=datetime.datetime.combine(alarmDate, self.attr['time'])
			if alarm < now: #already happened
				alarm = alarm + datetime.timedelta(days=1)
		else:
			alarm=datetime.datetime.combine(self.attr['date'], self.attr['time'])
		delta=alarm - now
		self.attr['delta'] = delta.seconds
		return delta.seconds
		
if __name__=="__main__":
	alarm = Alarm(hour=0,minute=15, fadeOverMinutes=15,badData=265)
	while True:
		alarm.run()
		time.sleep(1)