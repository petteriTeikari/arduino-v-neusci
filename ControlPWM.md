

# Introduction #

Here a simple introduction to LED intensity control using pulse-width modulation (PWM) with Arduino is presented. For more detailed discussion, following sites are recommended: [Secrets of Arduino PWM](http://www.arcfn.com/2009/07/secrets-of-arduino-pwm.html) by Ken Shirriff, [Adjusting PWM Frequencies](http://arduino.cc/playground/Main/TimerPWMCheatsheet) in Arduino, [ATmega328 (datasheet)](http://www.atmel.com/dyn/resources/prod_documents/doc8161.pdf) of Arduino Uno, and [PWM (Wikipedia)](http://en.wikipedia.org/wiki/Pulse-width_modulation)

<a href='http://www.youtube.com/watch?feature=player_embedded&v=-5eM74LlX9o' target='_blank'><img src='http://img.youtube.com/vi/-5eM74LlX9o/0.jpg' width='640' height=360 /></a>
<br><i>Demonstration of LED intensity control with PWM-signal from Arduino Uno using graphical Python front-end.</i>

<h1>Details</h1>

<h2>8-bit PWM (default)</h2>

To demonstrate the simplest case of LED light control needed in the <a href='http://en.wikipedia.org/wiki/Pupillary_light_reflex'>pupillometry</a> application described in the accompanying paper (<a href='http://dx.doi.org/'>DOI</a>). The Arduino sketch <code>arduinoLED-simpleGUI.ino</code> was developed based on the code provided by John Meichle for controlling RGB LEDs, as a part of his <a href='http://code.google.com/p/neuroelec/'>neuroelec</a> library for Arduino.<br>
<br>
<h3>GuiArduinoLED (Python, .py)</h3>

Arduino sketch (<code>GuiArduinoLED.ino</code>) needed to make Arduino respond to incoming serial communication for example from Python to signal desired changes in light intensity.<br>
<br>
<img src='https://lh3.googleusercontent.com/-8vjbtJM09js/T7PBNZvGRzI/AAAAAAAACg0/vlnCtrciqE8/s865/simpleLedGUI.gif' />
<br><i>Screen capture of the <a href='http://www.geany.org/'>Geany</a> view of the code with the GUI for 8-bit LED Control</i>

Main the <code>loop()</code> is given below:<br>
<br>
<pre><code><br>
void loop() {<br>
<br>
if(Serial.available() &gt;= 2){<br>
<br>
// The cases are given from the .py file (or some other frontend)<br>
switch( byte( Serial.read() )) {<br>
case 'r':<br>
ledOut_ch1 = Serial.read();<br>
break;<br>
case 'g':<br>
ledOut_ch2 = Serial.read();<br>
break;<br>
case 'b':<br>
ledOut_ch3 = Serial.read();<br>
break;<br>
case 'w':<br>
ledOut_ch4 = Serial.read();<br>
break;<br>
case 'v':<br>
ledOut_ch5 = Serial.read();<br>
case 'y':<br>
ledOut_ch6 = Serial.read();<br>
}<br>
}<br>
<br>
// write the PWM values now to all the channels<br>
analogWrite(ledOut_ch1Pin, ledOut_ch1);<br>
analogWrite(ledOut_ch2Pin, ledOut_ch2);<br>
analogWrite(ledOut_ch3Pin, ledOut_ch3);<br>
analogWrite(ledOut_ch4Pin, ledOut_ch4);<br>
analogWrite(ledOut_ch5Pin, ledOut_ch5);<br>
analogWrite(ledOut_ch6Pin, ledOut_ch6);<br>
<br>
// delay in each loop, in milliseconds<br>
delay(20); // going too low maybe lead to unstable behavior<br>
// default value 20 ms worked well<br>
<br>
}<br>
</code></pre>

Where <code>if(Serial.available() &gt;= 2){</code> checks whether serial commands are sent to Arduino, and if it has been sent then the program advances the <code>switch</code> conditional determining which channel of the 6 LED channels the user wanted to control.<br>
<br>
In the sending end of Python code, the code (<code>GuiArduinoLED.py</code>) looks like the following for one channel:<br>
<br>
<pre><code><br>
if name == "Channel1":<br>
self.ser.write("r" + chr(int(val)))<br>
</code></pre>

where the first character <code>"r"</code> specifies the desired controlled channel (<code>ledOut_ch1</code> variable in above Arduino sketch), to which is added (<code>+</code>) the desired value for the channel. The variable <code>val</code> is first converted to integer and then to character (8 bits).<br>
<br>
In the Arduino sketch all channels are now updated either with the updated value or the existing value independent of whether there was any change. The latency of 20 ms (with <a href='http://arduino.cc/en/Reference/delay'>delay</a> function) is added for more stable behavior of the loop.<br>
<br>
<h3>GuiArduinoLED (Matlab, .m, .fig)</h3>

A simple example for Matlab is provided to control the light intensity using the <a href='http://www.mathworks.com/matlabcentral/fileexchange/32374'>MATLAB Support Package for Arduino (aka ArduinoIO Package)</a> written by <a href='http://www.mathworks.com/matlabcentral/fileexchange/authors/76178'>Giampiero Campa</a>.<br>
<br>
Change the following line (line 42) of the <code>GuiArduinoLED.m</code> to match the serial port of your Arduino, typically like 'COM2' in Windows and '/dev/ttyACM0' in Linux. Also <b>remember</b> to upload the file <code>adiosrv.pde</code> sketch to your Arduino from the ArduinoIO package to make the demo work.<br>
<br>
<pre><code><br>
% initialize Arduino<br>
port = '/dev/tty.usbmodemfa131';<br>
</code></pre>

<img src='https://lh5.googleusercontent.com/-OC2fufYcYQc/T7PEWyWq3yI/AAAAAAAAChQ/p9a9BDv0PIc/s912/matlabScreenCap_GUIArduinoLED.gif' />
<br><i>Screencap of controlling LED intensity (1-channel example) with Arduino from LED</i>

<h3>GuiArduinoLED (LabVIEW, .vi)</h3>

Demonstration of how to use LabVIEW with Arduino is done using the <a href='http://sine.ni.com/nips/cds/view/p/lang/en/nid/209835'>LabVIEW Interface for Arduino</a> (LIFA) package provided by <a href='http://www.ni.com'>National Instruments</a> (NI, the company behind LabVIEW and vast selection of data acquisition instruments). The example is modified from the example "<a href='https://decibel.ni.com/content/docs/DOC-20044'>Simple LED</a>" provided by NI.<br>
<br>
<a href='http://www.youtube.com/watch?feature=player_embedded&v=EJyrLsVm9b8' target='_blank'><img src='http://img.youtube.com/vi/EJyrLsVm9b8/0.jpg' width='640' height=360 /></a><br>
<br><i>Demonstration of LED intensity control with PWM-signal from Arduino Uno using graphical LabVIEW VI front-end.</i>

You need to upload the Arduino sketch <a href='http://digital.ni.com/public.nsf/allkb/8C07747189606D148625789C005C2DD6'>LVIFA_base.pde</a> in order to make Arduino responsive to commands sent from LabVIEW:<br>
<br>
<img src='https://lh3.googleusercontent.com/-NueqQrYkbf4/T7PBKuQ169I/AAAAAAAACgA/WyecIpLp2L4/s800/LIFA_LVIFA_base.gif' />
<br><i>Screen capture of LVIFA_base.pde with its accompanying files.</i>

The block diagram of the Virtual Instrument ("LabVIEW program") looks the following:<br>
<br>
<img src='https://lh6.googleusercontent.com/-IjrTXwTHavg/T7PBLqmszbI/AAAAAAAACgM/T2kZ1BC1BXY/s912/guiArduinoLED_block.gif' />
<br><i>VI Block diagram for 8-bit LED intensity control with PWM for 2-channels</i>

And the front panel (GUI) as the following:<br>
<br>
<img src='https://lh3.googleusercontent.com/-KT89kQcCp6g/T7PBLgkh-cI/AAAAAAAACgI/5bfIPPStbXo/s413/guiArduinoFrontPanel.gif' />
<br><i>VI Front panel. The knob buttons control the intensity (0 to 255, 8 bit resolution) and from below them one can choose the pins to which the LEDs are connected. Debug loop count is useful when testing your Arduino and making sure that there is nothing slowing down the program and while loop is actually being executed.</i>

<h2>10-bit PWM</h2>

The default 8-bit resolution of Arduino PWM can be extended for limited amount of pins using for example the 3rd party <a href='http://www.arduino.cc/playground/Code/Timer1'>timer1</a> library developed by Jesse Tane. Before being able to upload the accompanying sketch <code>bit10PWM.ino</code> to Arduino board, the <code>timer1</code> need to be added for your system folder. For instructions how to do it, see <a href='http://arduino.cc/playground/Code/Library'>Arduino Playground's tutorial</a>. Alternatively you can just place the external library files to the same folder as your Arduino sketch (<a href='http://arduino.cc/en/Guide/Environment#libraries'>see how</a>).<br>
<br>
<h3>bit10PWM</h3>

The use of a library instead of "brute-force" low-level implementation simplifies the Arduino sketch (<code>bit10PWM.ino</code>) slightly as seen in the following segment:<br>
<br>
<pre><code><br>
void setup()<br>
{<br>
<br>
// Set the baud rate<br>
Serial.begin(57600);<br>
<br>
// Define output pin, and its mode<br>
pinMode(10, OUTPUT);<br>
<br>
// Initialize<br>
Timer1.initialize(PWM_period);         // initialize timer1, and set a 1/2 second period<br>
// Note that this breaks analogWrite() for digital pins 9 and 10 on Arduino.<br>
<br>
// Adjust the PWM period to be something else<br>
//Timer1.setPeriod(PWM_period);<br>
<br>
// Output the PWM<br>
Timer1.pwm(pinOut, led_dutyCycle_Out); // setup pwm on pin X<br>
<br>
Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt<br>
}<br>
</code></pre>

Now the library can be called with more 'human-readable' calls such as <code>Timer1.pwm()</code> with two input parameters <code>pinOut</code> specifying to which pin you have connected the anode of your LED, and `led_dutyCycle_Out' describing the wanted output intensity as 10-bit duty cycle (0 is 0% of the time ON, and 1023 is 100% of the time ON).<br>
<br>
The case handling of which LED channel want to be controlled from the Python GUI is the same as for the above described 8-bit PWM, but now special treatment is required for the value sent from Python as it is 10-bit and single characters are always 8-bit ones in serial communication.<br>
<br>
The sending Python code (<code>bit10PWM_demo.py</code>) looks like the following:<br>
<br>
<pre><code><br>
def on_changed(self, widget):<br>
<br>
val = widget.get_value()<br>
name = widget.get_name()<br>
<br>
<br>
c = int(val) &gt;&gt; 8  # The value of x shifted 8 bits to the right, creating coarse.<br>
f = int(val) % 256  # The remainder of x / 256, creating fine.<br>
print(c)<br>
print(f)<br>
<br>
if name == "Channel1":<br>
self.ser.write("c" + chr(int(c)) + chr(int(f)))<br>
else:<br>
print "ERROR: Invalid widget name, in on_changed function"<br>
</code></pre>

Where the 10-bit value is divided to coarse and fine part of the integer (<a href='http://stackoverflow.com/questions/3123371/splitting-a-16-bit-int-into-two-8-bit-ints-in-python'>see for further details</a> about splitting 16-bit integer into two 8-bit integers). And now three characters (<code>"c"</code>, <code>chr(int(c))</code>, <code>chr(int(f))</code>) are sent to Arduino, the first indicating the case, the second the coarse part, and the third part the fine part.<br>
<br>
The receiving end in the Arduino sketch (<code>bit10PWM.ino</code>) looks like this then:<br>
<br>
<pre><code><br>
// The cases are given from the .py file (or some other frontend)<br>
switch( byte( Serial.read() )) {<br>
case 'r':<br>
<br>
// now the intensity is encoded as two 8 bit integers<br>
// so we need to recombine<br>
<br>
// read the 8 bit ones<br>
int highByte = Serial.read();<br>
int lowByte = Serial.read();<br>
<br>
// combine<br>
// e.g.<br>
unsigned int led_dutyCycle_Out = highByte * 256 + lowByte;<br>
<br>
Serial.println(highByte);<br>
Serial.println(lowByte);<br>
Serial.println(led_dutyCycle_Out);<br>
break;<br>
}<br>
<br>
// Output the PWM<br>
Timer1.setPwmDuty(pinOut, led_dutyCycle_Out); // setup pwm on pin X<br>
</code></pre>

Now the <code>highByte</code> is used for the coarse value, and <code>lowByte</code> for the fine value to introduce common terminology for novice users (the variable names are also Arduino functions <a href='http://arduino.cc/en/Reference/HighByte'>highByte()</a> and <a href='http://arduino.cc/en/Reference/LowByte'>lowByte()</a>).<br>
<br>
<img src='https://lh6.googleusercontent.com/-ILOEhIx6-YI/T7PBKr6cciI/AAAAAAAACf8/bda4A4gaJzw/s1014/10bitTest.gif' />
<br><i>Screen capture of the <a href='http://www.geany.org/'>Geany</a> view of the code with the GUI for LED Control. Notice that now for each slider value two bytes are sent for Arduino (coarse and fine bytes)</i>

Now the PWM is updated inside the loop every time the user wants to change the LED intensity using the library call <code>.setPwmDuty</code>.<br>
<br>
<h1>Scientific Applications</h1>

<h2>Pupillometry</h2>

<ul><li>Viénot, F., S. Bailacq, and J. L. Rohellec. “The Effect of Controlled Photopigment Excitations on Pupil Aperture.” <i>Ophthalmic and Physiological Optics</i> 30, no. 5 (2010): 484–491. <a href='http://dx.doi.org/10.1111/j.1475-1313.2010.00754.x'>http://dx.doi.org/10.1111/j.1475-1313.2010.00754.x</a>.</li></ul>

<ul><li>Herbst, Kristina, Birgit Sander, Dan Milea, Henrik Lund-Andersen, and Aki Kawasaki. “Test–Retest Repeatability of the Pupil Light Response to Blue and Red Light Stimuli in Normal Human Eyes Using a Novel Pupillometer.” <i>Frontiers in Neurology</i> 2, no. 10 (2011). <a href='http://dx.doi.org/10.3389/fneur.2011.00010'>http://dx.doi.org/10.3389/fneur.2011.00010</a>.</li></ul>

<ul><li>Tsujimura, Sei‐ichi, and Yuta Tokuda. “Delayed Response of Human Melanopsin Retinal Ganglion Cells on the Pupillary Light Reflex.” <i>Ophthalmic and Physiological Optics</i> 31 (2011): 469–479. <a href='http://dx.doi.org/10.1111/j.1475-1313.2011.00846.x'>http://dx.doi.org/10.1111/j.1475-1313.2011.00846.x</a>.</li></ul>

<h2>Contrast sensitivity and glare measurement</h2>

<ul><li>van Rijn, L J, C Nischler, D Gamer, L Franssen, G de Wit, R Kaper, D Vonhoff, et al. “Measurement of Stray Light and Glare: Comparison of Nyktotest, Mesotest, Stray Light Meter, and Computer Implemented Stray Light Meter.” <i>British Journal of Ophthalmology</i> 89, no. 3 (March 1, 2005): 345 –351. <a href='http://dx.doi.org/10.1136/bjo.2004.044990'>http://dx.doi.org/10.1136/bjo.2004.044990</a>.</li></ul>

<ul><li>Aslam, Tariq M, David Haider, and Ian J Murray. “Principles of Disability Glare Measurement: An Ophthalmological Perspective.” <i>Acta Ophthalmologica Scandinavica</i> 85, no. 4 (June 1, 2007): 354–360. <a href='http://dx.doi.org/10.1111/j.1600-0420.2006.00860.x'>http://dx.doi.org/10.1111/j.1600-0420.2006.00860.x</a>.</li></ul>

<ul><li>van den Berg, Thomas J.T.P., Luuk Franssen, Bastiaan Kruijt, and Joris E. Coppens. “Psychophysics, Reliability, and Norm Values for Temporal Contrast Sensitivity Implemented on the Two Alternative Forced Choice C-Quant Device.” <i>Journal of Biomedical Optics</i> 16, no. 8 (2011): 085004. <a href='http://dx.doi.org/10.1117/1.3613922'>http://dx.doi.org/10.1117/1.3613922</a>.</li></ul>

<h2>Silent substitution</h2>

<ul><li>Ishihara, Makoto. “Versuch Einer Deutung Der Photoelektrischen Schwankungen Am Froschauge.” Pflüger, <i>Archiv Für Die Gesammte Physiologie Des Menschen Und Der Thiere</i> 114 (October 1906): 569–618. <a href='http://dx.doi.org/10.1007/BF01677147'>http://dx.doi.org/10.1007/BF01677147</a>.</li></ul>

<ul><li>Estévez, O., and H. Spekreijse. “The ‘Silent Substitution’ Method in Visual Research.” <i>Vision Research</i> 22, no. 6 (1982): 681–691. <a href='http://dx.doi.org/10.1016/0042-6989(82)90104-3'>http://dx.doi.org/10.1016/0042-6989(82)90104-3</a>.</li></ul>

<ul><li>Smith, Vivianne C., and Joel Pokorny. “The Design and Use of a Cone Chromaticity Space: A Tutorial.” <i>Color Research & Application</i> 21, no. 5 (1996): 375–383. <a href='http://dx.doi.org/10.1002/(SICI)1520-6378(199610)21:5<375::AID-COL6>3.0.CO;2-V'>http://dx.doi.org/10.1002/(SICI)1520-6378(199610)21:5&lt;375::AID-COL6&gt;3.0.CO;2-V</a>.</li></ul>

<ul><li>Fukuda, Yumi, Sei-Ichi Tsujimura, Shigekazu Higuchi, Akira Yasukouchi, and Takeshi Morita. “The ERG Responses to Light Stimuli of Melanopsin-expressing Retinal Ganglion Cells That Are Independent of Rods and Cones.” <i>Neuroscience Letters</i> 479, no. 3 (August 2, 2010): 282–286. <a href='http://dx.doi.org/10.1016/j.neulet.2010.05.080'>http://dx.doi.org/10.1016/j.neulet.2010.05.080</a>.</li></ul>

<ul><li>Klee, Sascha, Dietmar Link, Patrick Bessler, and Jens Haueisen. “Optoelectrophysiological Stimulation of the Human Eye Using Fundus-controlled Silent Substitution Technique.” <i>Journal of Biomedical Optics</i> 16, no. 1 (February 2011): 015002. <a href='http://dx.doi.org/10.1117/1.3528616'>http://dx.doi.org/10.1117/1.3528616</a>.</li></ul>

<ul><li>Viénot, Françoise, Hans Brettel, Tuong-Vi Dang, and Jean Le Rohellec. “Domain of Metamers Exciting Intrinsically Photosensitive Retinal Ganglion Cells (ipRGCs) and Rods.” <i>Journal of the Optical Society of America A</i> 29, no. 2 (February 1, 2012): A366–A376. <a href='http://dx.doi.org/10.1364/JOSAA.29.00A366'>http://dx.doi.org/10.1364/JOSAA.29.00A366</a>.</li></ul>

<h2>Other vision research</h2>

<ul><li>Demontis, Gian Carlo, Andrea Sbrana, Claudia Gargini, and Luigi Cervetto. “A Simple and Inexpensive Light Source for Research in Visual Neuroscience.” <i>Journal of Neuroscience Methods</i> 146, no. 1 (July 15, 2005): 13–21. <a href='http://dx.doi.org/10.1016/j.jneumeth.2005.01.007'>http://dx.doi.org/10.1016/j.jneumeth.2005.01.007</a>.</li></ul>

<ul><li>Woods, Russell L., Ahmed L. Rashed, Juan M. Benavides, and Robert H. Webb. “A Low-power, LED-based, High-brightness Anomaloscope.” <i>Vision Research</i> 46, no. 22 (October 2006): 3775–3781. <a href='http://dx.doi.org/10.1016/j.visres.2006.06.019'>http://dx.doi.org/10.1016/j.visres.2006.06.019</a>.</li></ul>

<ul><li>da Silva Pinto, Marcos Antonio, John Kennedy Schettino de Souza, Jerome Baron, and Carlos Julio Tierra-Criollo. “A Low-cost, Portable, Micro-controlled Device for Multi-channel LED Visual Stimulation.” <i>Journal of Neuroscience Methods</i> 197, no. 1 (April 15, 2011): 82–91. <a href='http://dx.doi.org/10.1016/j.jneumeth.2011.02.004'>http://dx.doi.org/10.1016/j.jneumeth.2011.02.004</a>.</li></ul>

<ul><li>Rogers, Bill, Yen-Yu I. Shih, Bryan De La Garza, Joseph M. Harrison, John Roby, and Timothy Q. Duong. “A Low Cost Color Visual Stimulator for fMRI.” <i>Journal of Neuroscience Methods</i> 204, no. 2 (March 15, 2012): 379–382. <a href='http://dx.doi.org/10.1016/j.jneumeth.2011.10.032'>http://dx.doi.org/10.1016/j.jneumeth.2011.10.032</a>.</li></ul>

<h2>Life sciences in general</h2>

<ul><li>Salzberg, B.M., P.V. Kosterin, M. Muschol, A.L. Obaid, S.L. Rumyantsev, Yu. Bilenko, and M.S. Shur. “An Ultra-stable Non-coherent Light Source for Optical Measurements in Neuroscience and Cell Physiology.” Journal of Neuroscience Methods 141, no. 1 (January 30, 2005): 165–169. <a href='http://dx.doi.org/10.1016/j.jneumeth.2004.06.009'>http://dx.doi.org/10.1016/j.jneumeth.2004.06.009</a>.</li></ul>

<ul><li>Albeanu, Dinu F., Edward Soucy, Tomokazu F. Sato, Markus Meister, and Venkatesh N. Murthy. “LED Arrays as Cost Effective and Efficient Light Sources for Widefield Microscopy.” <i>PLoS ONE</i> 3, no. 5 (May 14, 2008): e2146. <a href='http://dx.doi.org/10.1371/journal.pone.0002146'>http://dx.doi.org/10.1371/journal.pone.0002146</a>.</li></ul>

<ul><li>Zhong, Keke, Zhangwei Chen, Jing Huang, He Mao, and Kewei Hu. “A LED-induced confocal fluorescence detection system for quantitative PCR instruments.” In <i>2011 4th International Conference on Biomedical Engineering and Informatics (BMEI)</i>, 2:1014–1018. IEEE, 2011. <a href='http://dx.doi.org/10.1109/BMEI.2011.6098382'>http://dx.doi.org/10.1109/BMEI.2011.6098382</a>.</li></ul>

<h1>LED Illumination implementations</h1>

<h2>Open-Source / DIY Projects</h2>

<ul><li><a href='http://makezine.com/magazine/'>MAKE</a> | <a href='http://blog.makezine.com/2011/03/04/cheap-diy-gfp-green-fluorescent-protein-illuminator/'>Cheap DIY GFP (Green Fluorescent Protein) Illuminator</a></li></ul>

<ul><li><a href='http://neuroelec.com/'>neuroelec</a> » <a href='http://neuroelec.com/hp-rgb-led-shield/'>High Power RGB LED Shield</a></li></ul>

<ul><li><a href='http://www.chestersgarage.com/'>Chester's Garage</a>: <a href='http://www.chestersgarage.com/new-store/index.php?main_page=product_info&cPath=1&products_id=6'>Power LED Shield</a></li></ul>

<ul><li>Open-source LED current controller for biological applications from <a href='http://www.iorodeo.com/content/4-channel-led-current-controller-0'>IO Rodeo</a> (see the <a href='http://hg.iorodeo.com/buckpuck_current_controller'>source for BuckPuck-based LED controller</a>. IO Rodeo products employ also <a href='http://www.iorodeo.com/content/solid-state-relay-expansion-board-kit-arduino-nano'>Arduino Nano</a>.</li></ul>

<h2>Commercial products</h2>

<ul><li><a href='http://www.edmundoptics.com/illumination/led-illumination/'>LED Illumination | Edmund Optics</a>, Edmund Optics offers a variety of various LED illumination products including backlights, spotlights, mounts and more.</li></ul>

<ul><li><a href='http://www.thorlabs.com/navigation.cfm?guide_id=2101'>Thorlabs.com - Light Emitting Diodes (LED)</a>, Thorlabs provides a range of unmounted, mounted, and collimated LEDs that emit in the 340 - 4600 nm spectral range. A modulating source for Fluorescence Lifetime Imaging Microscopy is also available, as well as a Four-Channel collimated LED source designed for microscopy applications.