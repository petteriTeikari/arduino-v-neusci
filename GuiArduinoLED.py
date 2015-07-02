import gtk
import sys
import serial
import time

class PyApp(gtk.Window):

    def __init__(self):
	  super(PyApp, self).__init__()

	  self.serialPort = "/dev/ttyACM0"

	  self.set_title("LED Control Test")
	  self.set_size_request(260, 250)
	  self.set_position(gtk.WIN_POS_CENTER)
	  self.setup_serial()

	  # MAIN BOX
	  headerVbox = gtk.VBox(True,0)
	  headerLabel1 = gtk.Label("LED CONTROL")
	  headerLabel2 = gtk.Label("PWM Testing for Arduino Uno")
	  headerLabel3 = gtk.Label("Petteri Teikari")
	  headerVbox.pack_start(headerLabel1)
	  headerVbox.pack_end(headerLabel3)
	  headerVbox.pack_end(headerLabel2)

	  # CHANNEL 1
	  ch1Hbox = gtk.HBox(True,0)
	  ch1Label = gtk.Label("Ch1: (Pin 3)")
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
	  ch2Label = gtk.Label("Ch2: (Pin 5)")
	  ch2Hbox.pack_start(ch2Label)

	  ch2Scale = gtk.HScale()
	  ch2Scale.set_name("Channel2")
	  ch2Scale.set_range(0, 255)
	  ch2Scale.set_increments(1, 10)
	  ch2Scale.set_digits(0)
	  ch2Scale.set_size_request(160, 35)
	  ch2Scale.connect("value-changed", self.on_changed)
	  ch2Hbox.pack_end(ch2Scale)

      # CHANNEL 3
	  ch3Hbox = gtk.HBox(True,0)
	  ch3Label = gtk.Label("Ch3: (Pin 6)")
	  ch3Hbox.pack_start(ch3Label)

	  ch3Scale = gtk.HScale()
	  ch3Scale.set_name("Channel3")
	  ch3Scale.set_range(0, 255)
	  ch3Scale.set_increments(1, 10)
	  ch3Scale.set_digits(0)
	  ch3Scale.set_size_request(160, 35)
	  ch3Scale.connect("value-changed", self.on_changed)
	  ch3Hbox.pack_end(ch3Scale)

      # CHANNEL 4
	  ch4Hbox = gtk.HBox(True,0)
	  ch4Label = gtk.Label("Ch4: (Pin 9)")
	  ch4Hbox.pack_start(ch4Label)

	  ch4Scale = gtk.HScale()
	  ch4Scale.set_name("Channel4")
	  ch4Scale.set_range(0, 255)
	  ch4Scale.set_increments(1, 10)
	  ch4Scale.set_digits(0)
	  ch4Scale.set_size_request(160, 35)
	  ch4Scale.connect("value-changed", self.on_changed)
	  ch4Hbox.pack_end(ch4Scale)

	  # CHANNEL 5
	  ch5Hbox = gtk.HBox(True,0)
	  ch5Label = gtk.Label("Ch5: (Pin 10)")
	  ch5Hbox.pack_start(ch5Label)

	  ch5Scale = gtk.HScale()
	  ch5Scale.set_name("Channel5")
	  ch5Scale.set_range(0, 255)
	  ch5Scale.set_increments(1, 10)
	  ch5Scale.set_digits(0)
	  ch5Scale.set_size_request(160, 35)
	  ch5Scale.connect("value-changed", self.on_changed)
	  ch5Hbox.pack_end(ch5Scale)
	  
	  # CHANNEL 6
	  ch6Hbox = gtk.HBox(True,0)
	  ch6Label = gtk.Label("Ch6: (Pin 11)")
	  ch6Hbox.pack_start(ch6Label)

	  ch6Scale = gtk.HScale()
	  ch6Scale.set_name("Channel6")
	  ch6Scale.set_range(0, 255)
	  ch6Scale.set_increments(1, 10)
	  ch6Scale.set_digits(0)
	  ch6Scale.set_size_request(160, 35)
	  ch6Scale.connect("value-changed", self.on_changed)
	  ch6Hbox.pack_end(ch6Scale)

	  vbox = gtk.VBox(True,0)

	  vbox.pack_start(headerVbox)
	  vbox.pack_start(ch1Hbox)
	  vbox.pack_end(ch6Hbox)
	  vbox.pack_end(ch5Hbox)
	  vbox.pack_end(ch4Hbox)
	  vbox.pack_end(ch3Hbox)
	  vbox.pack_end(ch2Hbox)	  

	  self.add(vbox)
	  self.connect("destroy", lambda w: gtk.main_quit())
	  self.show_all()



    def on_changed(self, widget):

	  val = widget.get_value()
	  name = widget.get_name()

	  if name == "Channel1":
		self.ser.write("r" + chr(int(val)))
	  elif name == "Channel2":
		self.ser.write("g" + chr(int(val)))
	  elif name == "Channel3":
		self.ser.write("b" + chr(int(val)))
	  elif name == "Channel4":
		self.ser.write("w" + chr(int(val)))
	  elif name == "Channel5":
		self.ser.write("v" + chr(int(val)))
	  elif name == "Channel6":
		self.ser.write("y" + chr(int(val)))
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

PyApp()
gtk.main() 
