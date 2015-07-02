import gtk
import sys
import serial
import time


class PyApp(gtk.Window):

    def __init__(self):
	  super(PyApp, self).__init__()

	  self.serialPort = "/dev/ttyACM0"

	  self.set_title("Heterochromatic Test")
	  self.set_size_request(260, 250)
	  self.set_position(gtk.WIN_POS_CENTER)
	  self.setup_serial()

	  # MAIN BOX
	  headerVbox = gtk.VBox(True,0)
	  headerLabel1 = gtk.Label("Heterochromatic Test")
	  headerLabel2 = gtk.Label("Arduino Uno")
	  headerLabel3 = gtk.Label("Petteri Teikari")
	  headerVbox.pack_start(headerLabel1)
	  headerVbox.pack_end(headerLabel3)
	  headerVbox.pack_end(headerLabel2)

	  # CHANNEL 1
	  ch1Hbox = gtk.HBox(True,0)
	  ch1Label = gtk.Label("Ch1: (Pin 9)")
	  ch1Hbox.pack_start(ch1Label)

	  ch1Scale = gtk.HScale()
	  ch1Scale.set_name("Channel1")
	  ch1Scale.set_range(0, 255) # min,max of allowed values
	  ch1Scale.set_increments(1, 10) # minor,major increment steps
	  ch1Scale.set_digits(0) # number of digits
	  ch1Scale.set_size_request(160, 35) # physical size of slider (px)
	  ch1Scale.connect("value-changed", self.on_changed)
	  ch1Hbox.pack_end(ch1Scale)

	  # CHANNEL 2
	  ch2Hbox = gtk.HBox(True,0)
	  ch2Label = gtk.Label("Ch2: (Pin 11)")
	  ch2Hbox.pack_start(ch2Label)

	  ch2Scale = gtk.HScale()
	  ch2Scale.set_name("Channel2")
	  ch2Scale.set_range(0, 255)
	  ch2Scale.set_increments(1, 10)
	  ch2Scale.set_digits(0)
	  ch2Scale.set_size_request(160, 35)
	  ch2Scale.connect("value-changed", self.on_changed)
	  ch2Hbox.pack_end(ch2Scale)
	  
	  vbox = gtk.VBox(True,0)

	  vbox.pack_start(headerVbox)
	  vbox.pack_start(ch1Hbox)
	  vbox.pack_end(ch2Hbox)	  

	  self.add(vbox)
	  self.connect("destroy", lambda w: gtk.main_quit())
	  self.show_all()
    
    def receiving(ser):
		global last_received
		buffer = ''

		while True:
			# last_received = ser.readline()
			buffer += ser.read(ser.inWaiting())
			if '\n' in buffer:
				last_received, buffer = buffer.split('\n')[-2:]


    def on_changed(self, widget):

	  val = widget.get_value()
	  name = widget.get_name()

	  if name == "Channel1":
		self.ser.write("b" + chr(int(val)))
	  elif name == "Channel2":
		self.ser.write("g" + chr(int(val)))
	  else:
		print "ERROR: Invalid widget name, in on_changed function"
		

		
    def setup_serial(self):
	self.ser = serial.Serial()
	self.ser.setPort(self.serialPort)
	self.ser.baudrate = 57600
	self.ser.open()
	if (self.ser.isOpen()):
	  print "Serial Open"
	else:
	  print "Serial Closed"
	  

	# for reading digital input from Arduino in Python, one can check:
	
		# http://shallowsky.com/blog/hardware/ardmonitor.html
		
		# http://www.stealthcopter.com/blog/2010/02/python-interfacing-with-an-arduino/
		# http://dave-r.posterous.com/arduino-and-python
		# http://stackoverflow.com/questions/6300919/how-to-receive-socket-information-if-you-dont-know-how-much-data-is-coming-thro
		
		# with GUI: http://blog.wickeddevice.com/?p=191
		# with GUI: http://eli.thegreenplace.net/2009/08/07/a-live-data-monitor-with-python-pyqt-and-pyserial/
		
		# https://github.com/gregpinero/ArduinoPlot/blob/master/Arduino_Monitor.py
		# http://stackoverflow.com/questions/1093598/pyserial-how-to-read-last-line-sent-from-serial-device
		
		# arduinoscope: http://code.google.com/p/arduinoscope/

	# button in the sketch wired to the pin 7
		
PyApp()
gtk.main() 
