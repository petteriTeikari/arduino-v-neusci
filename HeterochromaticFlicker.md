

# Introduction #

Demonstration how to alternate between different LED channels as is needed to implement psychophysical techniques such as [heterochromatic flicker photometry](http://scholar.google.com/scholar?q=heterochromatic+flicker+photometry&hl=en&as_sdt=0&as_vis=1&oi=scholart&sa=X&ei=IDutT5WGKsPzsgbt_ZW8DA&ved=0CCUQgQMwAA) or [critical flicker fusion](http://scholar.google.com/scholar?hl=en&q=critical+flicker+fusion&btnG=Search&as_sdt=0%2C5&as_ylo=&as_vis=1) measurement. Arduino had been used similarly in the work of [Sun et al.](http://dx.doi.org/10.1364/BOE.1.000385) (Hillman Lab, Columbia University) to control LED lights in their open-source [SPLASSH](http://www.bme.columbia.edu/~hillman/SPLASSH.html) framework for multispectral _in vivo_ image acquisition. See [HeterochromaticFlicker#Scientific\_References](HeterochromaticFlicker#Scientific_References.md) for further information.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=75Jp-4oWJG8' target='_blank'><img src='http://img.youtube.com/vi/75Jp-4oWJG8/0.jpg' width='853' height=344 /></a>
<br><i>Demonstration of heterochromatic flicker technique using our proposed Arduino-based system and LabVIEW</i>

<h1>Details</h1>

The basic idea for the heterochromatic flicker photometry implementation is that there are <i>two</i> LEDs of different color (<i>heterochromatic</i>) which are switched on in counter-phase.<br>
<br>
<ol><li>LED 1 is ON for first 500 ms of 1000 ms while LED 2 is OFF during this period<br>
</li><li>LED 1 is OFF for the second 500 ms of 1000 ms while the LED 2 is ON during this period</li></ol>

This is considered to flicker at a frequency of 2 Hz maybe counter-intuitively to people with engineering background that might take this scheme to have to rectangular signals having a frequency of 1 Hz to be phase-shifted 180 degrees in respect to each other.<br>
<br>
In our implementation, the subject of the experiment is faced with a flickering annulus (ring of light) and is asked to adjust the intensity of only the other LED, until the <i>perception</i> of flicker is eliminated or minimized. The other LED is set at fixed level by the experimenter.<br>
<br>
The subject responses are read using digital inputs of which 2 are needed for 1-axis joystick indicating subject's desire to increase or decrease light intensity, and 1 digital input is needed to read the "confirm"button that the subject is instructed to press when he perceives no flickering. Thus, the joystick and the button are connected to a 5V power supply and ground via pulldown resistors and when pressed the Arduino detects that digital pin to be HIGH.<br>
<br>
We don't want to change the light intensity during the 500ms/500ms flicker period, thus the digital inputs are buffered during the whole second cycle and decision of whether subject asked is done based on these buffered samples. Thus, a sampling rate of 1 Hz for digital input in theory would be sufficient if subject pressed long enough the buttons and joystick.<br>
<br>
In our application, Arduino need to return the intensity values (duty cycles in other words) of the both LEDs when the subject presses the confirm button (which can be combined with some other data if needed in the Python GUI and written to disk as a text file for example using <a href='http://ttylog.sourceforge.net/'>ttylog</a>)<br>
<br>
<h2>Arduino sketch (heterochromaticFlicker.ino)</h2>

Compared to other examples (<a href='ControlPWM.md'>ControlPWM</a> and CueLightTask), additional conditional rules need to be introduced to make sure that the system behaves as we like it to behave:<br>
<br>
<h4>Counting the "button presses" during sampling period</h4>

We can count how many times the subject presses the button during the 1 sec sampling period. The resolution of counting depends on the digital sampling rate, in practice we get around 20 samples per second with current settings. We can then set a threshold of for counts that can be considered as a "real press", in our case only one was sufficient but in some other applications this check could be useful.<br>
<br>
For tutorial on debouncing the digital input (filter noise), see the tutorial of <a href='http://www.ladyada.net/learn/arduino/lesson5.html'>LadyAda</a> or our other example CueLightTask.<br>
<br>
The variables are initialized as following:<br>
<br>
<pre><code><br>
// i.e. the number of each channel pressed during<br>
// the 1 second period (2xflickerPeriod)<br>
int count_joystickUP = 0;<br>
int count_joystickDown = 0;<br>
int count_buttonPressed = 0;<br>
<br>
// if pressed or not<br>
int boolean_joystickUP = LOW;<br>
int boolean_joystickDown = LOW;<br>
int boolean_buttonPressed = LOW;<br>
</code></pre>

The basic <code>for</code>-loop in The Arduino sketch for the flickering scheme looks like the following:<br>
<br>
<pre><code><br>
for (int i = 1; i &lt;= 2; i++) {<br>
int numberOfDigitalSamples = 10;<br>
<br>
for (int j = 1; j &lt;= numberOfDigitalSamples; j++) {<br>
// Read the digital inputs<br>
digitalValues[0] = digitalRead(pinmap[0]);  // read input value<br>
digitalValues[1] = digitalRead(pinmap[1]);  // read input value<br>
digitalValues[2] = digitalRead(pinmap[2]);  // read input value<br>
<br>
// if you need to "filter" spurious button noise<br>
// check LadyAda's tutorial on digital inputs<br>
// and button debouncing:<br>
// http://www.ladyada.net/learn/arduino/lesson5.html<br>
<br>
// We can now simply calculate the number of HIGH signals<br>
// from each channel, and if the channels are very noisy<br>
// we can use some threshold values to determine whether<br>
// the button/joystick was actually pressed by the subject<br>
if (digitalValues[0] == HIGH) {<br>
count_joystickUP++;<br>
}<br>
<br>
if (digitalValues[1] == HIGH) {<br>
count_joystickDown++;<br>
}<br>
<br>
if (digitalValues[2] == HIGH) {<br>
count_buttonPressed++;<br>
}<br>
<br>
// delay in each loop, in milliseconds<br>
delay(flickerPeriod / numberOfDigitalSamples); // e.g. 500 ms<br>
<br>
}<br>
<br>
// write the PWM values now to all the channels<br>
if (i == 1) {<br>
analogWrite(ledOut_ch1Pin, ledOut_ch1);<br>
analogWrite(ledOut_ch2Pin, 0);<br>
}<br>
else {<br>
analogWrite(ledOut_ch1Pin, 0);<br>
analogWrite(ledOut_ch2Pin, ledOut_ch2);<br>
}<br>
<br>
}<br>
</code></pre>

In the code we count the number of <code>HIGH's per channel (variables </code>count<code>) and end of each loop delay is introduced which is as long as the desired flicker period). After the </code>for<code>-loop (~500 ms) the duty cycles of the LEDs are updated, so that on the first execution of the </code>for' (when <code>i == 1</code>) the LED 1 is switched on with desired duty cycle, and on second execution the LED 2 with desired duty cycle while the other LED is off (duty cycle of <code>0</code>).<br>
<br>
After this we can compare the counts again the set threshold for number of presses:<br>
<br>
<pre><code><br>
// Evaluate whether the buttons/joysticks are really pressed<br>
<br>
if (count_joystickUP &gt;= pressThreshold) {<br>
boolean_joystickUP = HIGH;<br>
ledOut_ch1++;<br>
}<br>
<br>
// Evaluate whether the buttons are really pressed<br>
if (count_joystickDown &gt;= pressThreshold) {<br>
boolean_joystickDown = HIGH;<br>
ledOut_ch1--;<br>
}<br>
<br>
// Evaluate whether the buttons are really pressed<br>
if (count_buttonPressed &gt;= pressThreshold) {<br>
boolean_buttonPressed = HIGH;<br>
}<br>
</code></pre>

<h4>Limiting how often subject can confirm</h4>

In practice, it takes several tens of seconds for the subject to find the point on which he confirms the "nulling" of flicker, thus it is not possible that subject would like to confirm every second. In practice this can occur if the subject presses the confirm button very long or near the end of the 1 sec sampling period, the button press extending to the subsequent 1 sec sampling period.<br>
<br>
In the Arduino sketch (<code>heterochromaticFlicker.ino</code>), this is implemented as following, first in the setup of the sketch<br>
<br>
<pre><code><br>
// additionally we want to limit the possible<br>
// frequency of "confirm presses" as it not possible<br>
// in the study paradigm really to confirm the flicker<br>
// fusion every second<br>
#define noOfHistorysSamples 6<br>
int confirmButtonHistory[noOfHistorysSamples];<br>
</code></pre>

Which defines a vector with <code>noOfHistorySamples</code> (6 samples now) integer values called <code>confirmButtonHistory</code>. Above we defined the rules whether the button (or joystick) was <i>really</i> pressed, and we can extend this to check how many previous presses there were during previous cycles as following:<br>
<br>
<pre><code><br>
// Add the newly read boolean_buttonPressed after<br>
// making "room" for it using a temporary array<br>
<br>
int tempHistory[noOfHistorysSamples];<br>
for (int k = 1; k &lt;= noOfHistorysSamples-1; k++) {<br>
tempHistory[k-1] = confirmButtonHistory[k];<br>
}<br>
tempHistory[noOfHistorysSamples-1] = boolean_buttonPressed;<br>
<br>
for (int k = 0; k &lt; noOfHistorysSamples; k++) {<br>
confirmButtonHistory[k] = tempHistory[k];<br>
}<br>
<br>
// calculate the number of presses in the history<br>
int buttonCount = 0;<br>
for (int l = 0; l &lt; noOfHistorysSamples; l++) {<br>
if (confirmButtonHistory[l] == HIGH) {<br>
buttonCount++;<br>
}<br>
}<br>
<br>
// now after HISTORY CHECK if only one button press<br>
// is found, then we can say that sufficient time has<br>
// passed since the previous button press<br>
if (buttonCount == 1 &amp;&amp; boolean_buttonPressed == HIGH) {<br>
boolean_buttonPressedAfterHistoryCheck = HIGH;<br>
}<br>
</code></pre>

So the above introduced <code>noOfHistorySamples</code> is the scope of the history examined (when sampling period is 1 sec, the scope in second is 6 times 1 sec - 6 seconds) during which no confirm button is allowed. Only when when number of button presses is 1 (<code>buttonCount == 1</code>) and a real button press is detected (<code>boolean_buttonPressed == HIGH</code>) final button press is accepted (<code>boolean_buttonPressedAfterHistoryCheck = HIGH</code>).<br>
<br>
<a href='http://www.youtube.com/watch?feature=player_embedded&v=ypg4hBQ07WI' target='_blank'><img src='http://img.youtube.com/vi/ypg4hBQ07WI/0.jpg' width='853' height=344 /></a><br>
<i>Example of button confirmation, being limited by the <code>noOfHistorySamples</code></i>

Note that in the current implementation joystick presses are not limited in similar fashion as it was not needed in our experimental paradigm.<br>
<br>
Finally the LED intensity valued are returned for example to be read using Python:<br>
<br>
<pre><code><br>
// PRINT NOW THE VALUES Out if button is pressed<br>
<br>
if (boolean_buttonPressedAfterHistoryCheck == HIGH) {<br>
<br>
// Upon confirm we want to know to which intensity<br>
// values the user confirmed the fusion,<br>
// so we return then<br>
<br>
Serial.print(ledOut_ch1);<br>
Serial.print("\t");    // prints a tab<br>
Serial.print(ledOut_ch2);<br>
Serial.print("\t");    // prints a tab<br>
Serial.print("\n");    // prints a line change<br>
<br>
}<br>
</code></pre>

The correct behavior of the Arduino sketch can now be monitored using the <a href='http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1274930169'>Serial Monitor</a> of the <a href='http://arduino.cc/en/Guide/Environment'>Arduino IDE</a> without having to having to have a software front-end.<br>
<br>
<h3>Arduino Debug (	heterochromaticFlickerExclIncrementDecrements.ino)</h3>

Slight modification of the actual 'heterochromaticFlicker.ino'. The detection of joystick presses do not know lead to increase/decrease of LED intensity. This is useful when you don't have anything connected to Arduino Pins when the state of the digital pins are not necessarily LOW and the Arduino can then behave as it would receive "phantom" joystick presses from user.<br>
<br>
<pre><code><br>
if (count_joystickUP &gt;= pressThreshold) {<br>
boolean_joystickUP = HIGH;<br>
// ledOut_ch1++;<br>
}<br>
<br>
// Evaluate whether the buttons are really pressed<br>
if (count_joystickDown &gt;= pressThreshold) {<br>
boolean_joystickDown = HIGH;<br>
// ledOut_ch1--;<br>
}<br>
</code></pre>

Now the user control of the LED is hard-wired only the 'ch1', but could be modified to be switch-controlled from the Python GUI for example. This was omitted from the example from the sake of simplicity.<br>
<br>
<h2>Python GUI (heterochromaticFlicker.py)</h2>

The basic implementation of Python GUI is simple with only two sliders for adjusting the intensity of the flickering lights:<br>
<br>
<pre><code><br>
def on_changed(self, widget):<br>
<br>
val = widget.get_value()<br>
name = widget.get_name()<br>
<br>
if name == "Channel1":<br>
self.ser.write("b" + chr(int(val)))<br>
elif name == "Channel2":<br>
self.ser.write("g" + chr(int(val)))<br>
else:<br>
print "ERROR: Invalid widget name, in on_changed function"<br>
</code></pre>

The case conditional to identify which LED channel want to be modified is implemented similarly as for other examples (<a href='ControlPWM.md'>ControlPWM</a> and CueLightTask)<br>
<br>
<img src='https://lh3.googleusercontent.com/-Md8uRMCKwao/T7PBMcyi9VI/AAAAAAAACgk/OZeN1b3VUdw/s807/heterochromaticTest.gif' />
<br><i>Screen capture of the heterochromatic flicker Python GUI.</i>

<h2>LabVIEW VI (heterochromaticFlicker.vi)</h2>

As shown in <a href='ControlPWM.md'>ControlPWM</a>, LabVIEW can be used to control the LEDs using Arudino.<br>
<br>
Demonstration of how to use LabVIEW with Arduino is done using the <a href='http://sine.ni.com/nips/cds/view/p/lang/en/nid/209835'>LabVIEW Interface for Arduino</a> (LIFA) package provided by <a href='http://www.ni.com'>National Instruments</a> (NI, the company behind LabVIEW and vast selection of data acquisition instruments). The example is modified from the example "<a href='https://decibel.ni.com/content/docs/DOC-20044'>Simple LED</a>" provided by NI.<br>
<br>
You need to upload the Arduino sketch <a href='http://digital.ni.com/public.nsf/allkb/8C07747189606D148625789C005C2DD6'>LVIFA_base.pde</a> in order to make Arduino responsive to commands sent from LabVIEW:<br>
<br>
<img src='https://lh3.googleusercontent.com/-NueqQrYkbf4/T7PBKuQ169I/AAAAAAAACgA/WyecIpLp2L4/s800/LIFA_LVIFA_base.gif' />
<br><i>Screen capture of LVIFA_base.pde with its accompanying files.</i>

The LabVIEW Virtual Instrument block diagram for the heterochromatic flicker looks like that without user inputs (to increment/decrement LED intensity):<br>
<br>
<img src='https://lh4.googleusercontent.com/-7T5rILQRygs/T7PBMSrXqTI/AAAAAAAACgc/yNwxybrTFjo/s1102/heterochromaticFlickerBlock.gif' />
<br><i>Screen capture of the block diagram of LabVIEW heterochromatic flicker photometry.</i>

The corresponding front panel GUI looks as following:<br>
<br>
<img src='https://lh5.googleusercontent.com/-rr4_ggzz8eY/T7PBL_6K40I/AAAAAAAACgY/qgSPDmww_54/s541/heterochromaticFlicker.gif' />

<h1>Scientific References</h1>

<h2>Heterochromatic flicker photometry</h2>

<ul><li>Wagner, Gunther, and Robert M. Boynton. “Comparison of Four Methods of Heterochromatic Photometry.” <i>Journal of the Optical Society of America 62</i>, no. 12 (December 1, 1972): 1508–1515. <a href='http://dx.doi.org/10.1364/JOSA.62.001508'>http://dx.doi.org/10.1364/JOSA.62.001508</a>.</li></ul>

<ul><li>Silver, Priscilla H. “A Grey Squirrel Spectral Sensitivity by Heterochromatic Flicker and Its Implications.” <i>Vision Research</i> 16, no. 11 (1976): 1235 – 1239. <a href='http://dx.doi.org/10.1016/0042-6989(76)90047-X'>http://dx.doi.org/10.1016/0042-6989(76)90047-X</a>.</li></ul>

<ul><li>Kelly, D. H., and D. van Norren. “Two-band Model of Heterochromatic Flicker.” <i>Journal of the Optical Society of America</i> 67, no. 8 (1977): 1081–1091. <a href='http://dx.doi.org/10.1364/JOSA.67.001081'>http://dx.doi.org/10.1364/JOSA.67.001081</a>.</li></ul>

<ul><li>Werner, John S., Seaneen K. Donnelly, and Reinhold Kliegl. “Aging and Human Macular Pigment Density.” <i>Vision Research 27</i>, no. 2 (1987): 257–268. <a href='http://dx.doi.org/10.1016/0042-6989(87)90188-X'>http://dx.doi.org/10.1016/0042-6989(87)90188-X</a>.</li></ul>

<ul><li>Lee, B B, P R Martin, and A Valberg. “The Physiological Basis of Heterochromatic Flicker Photometry Demonstrated in the Ganglion Cells of the Macaque Retina.” <i>The Journal of Physiology</i> 404, no. 1 (October 1, 1988): 323 –347. <a href='http://www.ncbi.nlm.nih.gov/pubmed/3253435'>http://www.ncbi.nlm.nih.gov/pubmed/3253435</a>.</li></ul>

<ul><li>van den Berg, Thomas J. T. P, J.K. Ijspeert, and P.W.T. de Waard. “Dependence of Intraocular Straylight on Pigmentation and Light Transmission Through the Ocular Wall.” <i>Vision Research</i> 31, no. 7–8 (1991): 1361–1367. <a href='http://dx.doi.org/10.1016/0042-6989(91)90057-C'>http://dx.doi.org/10.1016/0042-6989(91)90057-C</a>.</li></ul>

<ul><li>Kraft, James M., and John S. Werner. “Spectral Efficiency Across the Life Span: Flicker Photometry and Brightness Matching.” <i>Journal of the Optical Society of America A</i> 11, no. 4 (April 1, 1994): 1213–1221. <a href='http://dx.doi.org/10.1364/JOSAA.11.001213'>http://dx.doi.org/10.1364/JOSAA.11.001213</a>.</li></ul>

<ul><li>Bone, Richard A., and John T. Landrum. “Heterochromatic Flicker Photometry.” <i>Archives of Biochemistry and Biophysics</i> 430, no. 2 (October 15, 2004): 137–142. <a href='http://dx.doi.org/10.1016/j.abb.2004.04.003'>http://dx.doi.org/10.1016/j.abb.2004.04.003</a>.</li></ul>

<ul><li>Wooten, Billy R, Billy R Hammond, and Lisa M Renzi. “Using Scotopic and Photopic Flicker to Measure Lens Optical Density.” <i>Ophthalmic & Physiological Optics</i> 27, no. 4 (July 2007): 321–328. <a href='http://dx.doi.org/10.1111/j.1475-1313.2007.00489.x'>http://dx.doi.org/10.1111/j.1475-1313.2007.00489.x</a>.</li></ul>

<ul><li>Hagen, Stefan, Ilse Krebs, Carl Glittenberg, and Susanne Binder. “Repeated Measures of Macular Pigment Optical Density to Test Reproducibility of Heterochromatic Flicker Photometry.” <i>Acta Ophthalmologica</i> 88, no. 2 (March 1, 2010): 207–211. <a href='http://dx.doi.org/10.1111/j.1755-3768.2008.01418.x'>http://dx.doi.org/10.1111/j.1755-3768.2008.01418.x</a>.</li></ul>

<ul><li>Bartlett, Hannah, Olivia Howells, and Frank Eperjesi. “The Role of Macular Pigment Assessment in Clinical Practice: a Review.” Clinical and Experimental Optometry 93, no. 5 (September 1, 2010): 300–308. <a href='http://dx.doi.org/10.1111/j.1444-0938.2010.00499.x'>http://dx.doi.org/10.1111/j.1444-0938.2010.00499.x</a>.</li></ul>

<ul><li>Barboni, Mirella Telles Salgueiro, Gobinda Pangeni, Dora Fix Ventura, Folkert Horn, and Jan Kremers. “Heterochromatic Flicker Electroretinograms Reflecting Luminance and Cone Opponent Activity in Glaucoma Patients.” <i>Investigative Ophthalmology & Visual Science</i> (July 21, 2011). <a href='http://dx.doi.org/10.1167/iovs.11-7538'>http://dx.doi.org/10.1167/iovs.11-7538</a>.</li></ul>

<ul><li>O’Brien, Kevin, Bill Smollon, Bill Wooten, and Billy Hammond. “Determining Heterochromatic Flicker Photometry Frequency for Macular Pigment Optical Densitometry by Critical Flicker Fusion Frequency.” <i>Journal of Vision</i> 11, no. 15 (December 27, 2011): 55–55. <a href='http://dx.doi.org/10.1167/11.15.55'>http://dx.doi.org/10.1167/11.15.55</a>.</li></ul>

<ul><li>van den Berg, Thomas J.T.P., Luuk Franssen, Bastiaan Kruijt, and Joris E. Coppens. “Psychophysics, Reliability, and Norm Values for Temporal Contrast Sensitivity Implemented on the Two Alternative Forced Choice C-Quant Device.” <i>Journal of Biomedical Optics</i> 16, no. 8 (2011): 085004. <a href='http://dx.doi.org/10.1117/1.3613922'>http://dx.doi.org/10.1117/1.3613922</a>.</li></ul>

<ul><li>Brown, Timothy M., Sei-ichi Tsujimura, Annette E. Allen, Jonathan Wynne, Robert Bedford, Graham Vickery, Anthony Vugler, and Robert J. Lucas. “Melanopsin-Based Brightness Discrimination in Mice and Humans.” Current Biology 22, no. 12 (June 19, 2012): 1134–1141. <a href='http://dx.doi.org/10.1016/j.cub.2012.04.039'>http://dx.doi.org/10.1016/j.cub.2012.04.039</a>.</li></ul>

<h3>Commercial products</h3>

<ul><li><a href='http://www.oculus.de/en/sites/detail_ger.php?page=499'>Oculus C-Quant</a> straylight meter, which was further modified by <a href='http://dx.doi.org/10.1117/1.3613922'>van den Berg et al., 2011</a> with custom Matlab code.</li></ul>

<ul><li><a href='http://www.optos.com/en/Products/Diagnostic-instruments/Perimeter/'>Perimeters</a> from <a href='http://www.optos.com/en/'>Optos</a> for <a href='http://www.perimetry.org/PerimetryHistory/flicker/index.htm'>flicker perimetry</a> applications for example.</li></ul>

<ul><li><a href='http://www.macuscope.com'>Macuscope</a>, The MacuScope is a "heterochromatic flicker photometer" - the gold standard for measuring macular protective pigment density (MPPD). See <a href='http://www.youtube.com/watch?v=qKHWIHu7UsM'>YouTube video</a> to see the Macuscope in action.</li></ul>

<ul><li>MPS 9000 Macular Pigment Screener, commercially available? Evaluated in the following papers e.g.: <a href='http://dx.doi.org/10.1111/j.1444-0938.2010.00499.x'>Bartlett et al., 2010</a>, <a href='http://dx.doi.org/10.1111/j.1755-3768.2011.02294.x'>Loughman et al., 2011</a> and <a href='http://dx.doi.org/10.1136/bjo.2010.195321'>Murray et al., 2011</a></li></ul>

<h2>Critical Flicker Fusion (CFF) measurements</h2>

<ul><li>Porter, T. C. “Contributions to the Study of Flicker. Paper II.” <i>Proceedings of the Royal Society of London</i> 70 (January 1, 1902): 313–329.</li></ul>

<ul><li>Ives, Herbert E. “Critical Frequency Relations in Scotopic Vision.” <i>Journal of the Optical Society of America</i> 6, no. 3 (May 1, 1922): 254–267. <a href='http://dx.doi.org/10.1364/JOSA.6.000254'>http://dx.doi.org/10.1364/JOSA.6.000254</a>.</li></ul>

<ul><li>Hecht, S, S Shlaer, and C D Verrijp. “Intermittent Stimulation by Light: II. The Measurement of Critical Fusion Frequency for the Human Eye.” <i>The Journal of General Physiology</i> 17, no. 2 (November 20, 1933): 237–249. <a href='http://www.ncbi.nlm.nih.gov/pubmed/19872776'>http://www.ncbi.nlm.nih.gov/pubmed/19872776</a>.</li></ul>

<ul><li>Hecht, S, and S Shlaer. “Intermittent Stimulation by Light: V. The Relation Between Intensity and Critical Frequency for Different Parts of the Spectrum.” <i>The Journal of General Physiology</i> 19, no. 6 (July 20, 1936): 965–977. <a href='http://www.ncbi.nlm.nih.gov/pubmed/19872976'>http://www.ncbi.nlm.nih.gov/pubmed/19872976</a>.</li></ul>

<ul><li>De Lange DZN, H. “Relationship Between Critical Flicker-Frequency and a Set of Low-Frequency Characteristics of the Eye.” <i>Journal of the Optical Society of America</i> 44, no. 5 (May 1, 1954): 380–388. <a href='http://dx.doi.org/10.1364/JOSA.44.000380'>http://dx.doi.org/10.1364/JOSA.44.000380</a>.</li></ul>

<ul><li>Truss, Carroll Vance. “Chromatic Flicker Fusion Frequency as a Function of Chromaticity Difference.” <i>Journal of the Optical Society of America</i> 47, no. 12 (December 1, 1957): 1130–1134. <a href='http://dx.doi.org/10.1364/JOSA.47.001130'>http://dx.doi.org/10.1364/JOSA.47.001130</a>.</li></ul>

<ul><li>Brown, JL. “Flicker an Intermittent Stimulation.” In Vision and Visual Perception, edited by Clarence Henry Graham. New York: John Wiley & Sons Inc, 1965.</li></ul>

<ul><li>Ong, J, and T Wong. “Effect of Ametropias on Critical Fusion Frequency.” American Journal of Optometry and Archives of American Academy of Optometry 48, no. 9 (September 1971): 736–739. <i>http://www.ncbi.nlm.nih.gov/pubmed/5292020</i>.</li></ul>

<ul><li>Conner, J D. “The Temporal Properties of Rod Vision.” <i>The Journal of Physiology</i> 332 (November 1982): 139–155. <a href='http://www.ncbi.nlm.nih.gov/pubmed/7153925'>http://www.ncbi.nlm.nih.gov/pubmed/7153925</a>.</li></ul>

<ul><li>Nygaard, R.W., and T.E. Frumkes. “Frequency Dependence in Scotopic Flicker Sensitivity.” <i>Vision Research</i> 25, no. 1 (1985): 115–127. <a href='http://dx.doi.org/10.1016/0042-6989(85)90085-9'>http://dx.doi.org/10.1016/0042-6989(85)90085-9</a>.</li></ul>

<ul><li>Raninen, Antti, and Jyrki Rovamo. “Perimetry of Critical Flicker Frequency in Human Rod and Cone Vision.” <i>Vision Research</i> 26, no. 8 (1986): 1249–1255. <a href='http://dx.doi.org/10.1016/0042-6989(86)90105-7'>http://dx.doi.org/10.1016/0042-6989(86)90105-7</a>.</li></ul>

<ul><li>Curran, Stephen, and John P. Wattis. “Critical Flicker Fusion Threshold: a Useful Research Tool in Patients with Alzheimer’s Disease.” <i>Human Psychopharmacology: Clinical and Experimental</i> 13, no. 5 (1998): 337–355. <a href='http://dx.doi.org/10.1002/(SICI)1099-1077(199807)13:5<337::AID-HUP7>3.0.CO;2-P'>http://dx.doi.org/10.1002/(SICI)1099-1077(199807)13:5&lt;337::AID-HUP7&gt;3.0.CO;2-P</a>.</li></ul>

<ul><li>Diana Racene, "Computerized device for critical flicker fusion frequency determination", <i>Proc. SPIE</i> 5123, 349 (2003); <a href='http://dx.doi.org/10.1117/12.517043'>http://dx.doi.org/10.1117/12.517043</a></li></ul>

<ul><li>Vianya-Estopà, Marta, William A Douthwaite, Konrad Pesudovs, Bruce A Noble, and David B Elliott. “Development of a Critical Flicker/fusion Frequency Test for Potential Vision Testing in Media Opacities.” <i>Optometry and Vision Science</i> 81, no. 12 (December 2004): 905–910. <a href='http://www.ncbi.nlm.nih.gov/pubmed/15592114'>http://www.ncbi.nlm.nih.gov/pubmed/15592114</a>.</li></ul>

<ul><li>Hammond, Billy R Jr., and Billy R Wooten. “CFF Thresholds: Relation to Macular Pigment Optical Density.” <i>Ophthalmic and Physiological Optics</i> 25, no. 4 (July 1, 2005): 315–319. <a href='http://dx.doi.org/10.1111/j.1475-1313.2005.00271.x'>http://dx.doi.org/10.1111/j.1475-1313.2005.00271.x</a>.</li></ul>

<ul><li>Daetwyler, Kathleen; Borgmeier, Paul; Digre, Kathleen B; Warner, Judith; Creel, Donnell; Weinberg, David: Katz, Bradley J. "A Novel Device to Measure Critical Flicker Fusion Frequency" NANOS 2007: Poster Presentations, <a href='http://content.lib.utah.edu/cdm4/item_viewer.php?CISOROOT=/ehsl-nam&CISOPTR=991#metajump'>Link</a> (accessed May 17 2012)</li></ul>

<ul><li>Gregori, Bruno, Odysseas Papazachariadis, Alfonsa Farruggia, and Neri Accornero. “A Differential Color Flicker Test for Detecting Acquired Color Vision Impairment in Multiple Sclerosis and Diabetic Retinopathy.” <i>Journal of the Neurological Sciences</i> 300, no. 1–2 (January 15, 2011): 130–134. <a href='http://dx.doi.org/10.1016/j.jns.2010.09.002'>http://dx.doi.org/10.1016/j.jns.2010.09.002</a>.</li></ul>

<ul><li>O’Brien, Kevin, Bill Smollon, Bill Wooten, and Billy Hammond. “Determining Heterochromatic Flicker Photometry Frequency for Macular Pigment Optical Densitometry by Critical Flicker Fusion Frequency.” <i>Journal of Vision</i> 11, no. 15 (December 27, 2011): 55–55. <a href='http://dx.doi.org/10.1167/11.15.55'>http://dx.doi.org/10.1167/11.15.55</a>.</li></ul>

<ul><li>Bowles, Kristen E, and Timothy W Kraft. “ERG Critical Flicker Frequency Assessment in Humans.” <i>Advances in Experimental Medicine and Biology</i> 723 (2012): 503–509. <a href='http://dx.doi.org/10.1007/978-1-4614-0631-0_63'>http://dx.doi.org/10.1007/978-1-4614-0631-0_63</a>.</li></ul>

<h3>Commercial products</h3>

<ul><li><a href='http://www.schuhfried.com/vienna-test-system-vts/all-tests-from-a-z/test/flim-flickerfusion-frequency/'>FLIM Flicker/Fusion Frequency</a> device by <a href='http://www.schuhfried.com/'>SCHUHFRIED GmbH</a></li></ul>

<ul><li>[www.lafayettelifesciences.com/product_detail.asp?ItemID=36 Flicker Fusion System Model 12021] from <a href='http://www.lafayetteinstrument.com/'>Lafayette Instrument</a>