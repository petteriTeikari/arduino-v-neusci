import gtk
import sys
import serial
import time

class PyApp(gtk.Window):

    def __init__(self):
	  super(PyApp, self).__init__()

	  self.serialPort = "/dev/ttyACM0"

	  self.set_title("LED Control Test")
	  self.set_size_request(260, 150)
	  self.set_position(gtk.WIN_POS_CENTER)
	  self.setup_serial()

	  # MAIN BOX
	  headerVbox = gtk.VBox(True,0)
	  headerLabel1 = gtk.Label("LED CONTROL")
	  headerLabel2 = gtk.Label("10bit PWM Testing for Arduino Uno")
	  headerLabel3 = gtk.Label("Petteri Teikari")
	  headerVbox.pack_start(headerLabel1)
	  headerVbox.pack_end(headerLabel3)
	  headerVbox.pack_end(headerLabel2)

	  # CHANNEL 1
	  ch1Hbox = gtk.HBox(True,0)
	  ch1Label = gtk.Label("LED (Pin 9)")
	  ch1Hbox.pack_start(ch1Label)

	  ch1Scale = gtk.HScale()
	  ch1Scale.set_name("Channel1")
	  ch1Scale.set_range(0, 1023) # min,max of allowed values
	  ch1Scale.set_increments(1, 10) # minor,major increment steps
	  ch1Scale.set_digits(0) # number of digits
	  ch1Scale.set_size_request(160, 35) # physical size of slider (px)
	  ch1Scale.connect("value-changed", self.on_changed)
	  ch1Hbox.pack_end(ch1Scale)


	  vbox = gtk.VBox(True,0)

	  vbox.pack_start(headerVbox)
	  vbox.pack_start(ch1Hbox) 

	  self.add(vbox)
	  self.connect("destroy", lambda w: gtk.main_quit())
	  self.show_all()



    def on_changed(self, widget):

	  val = widget.get_value()
	  name = widget.get_name()
	  
	  
	  c = int(val) >> 8  # The value of x shifted 8 bits to the right, creating coarse.
	  f = int(val) % 256  # The remainder of x / 256, creating fine.
	  print(c)
	  print(f)

	  if name == "Channel1":
		self.ser.write("c" + chr(int(c)) + chr(int(f)))			
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
