

# Introduction #

Here is demonstrated how the Arduino can be made to react to digital input by executing a certain function. In our particular case the digital input(s) are the call to initialize experimental session, and the indication that the mouse has received a reward. In both cases, the behavior of the system from a technical perspective is similar, Arduino has to react to 5V signal coming from a sensor, button, etc. This is also referred as TTL ([Transistor–transistor logic](http://en.wikipedia.org/wiki/Transistor%E2%80%93transistor_logic)).

<a href='http://www.youtube.com/watch?feature=player_embedded&v=FUZlJP7V2ME' target='_blank'><img src='http://img.youtube.com/vi/FUZlJP7V2ME/0.jpg' width='560' height=315 /></a>

<br><i>See the video demonstrating the functionality of this Arduino sketch</i>

In practice, the threshold for interpreting the digital input as HIGH and LOW is ~3 V given by <a href='http://arduino.cc/en/Reference/Constants'>Arduino - Constants</a>, thus making the Arduino directly comparable with 3.3 V logic circuits in addition to 5 V logic without hardware modifications (and saving a lot of effort and time).<br>
<br>
Default implementation of Arduino is powered by 5 V power, but if one desires to make it compatible for more recent devices running on 3.3 V, one might just replace the on-board regulator of the Arduino as demonstrated at <a href='http://www.ladyada.net/library/arduino/3v3_arduino.html'>LadyAda</a>.<br>
<br>
For analysis of the timing precision of the Arduino system in behavioral experiments, one is referred to the following publication of Alessandro D’Ausilio:<br>
<br>
<blockquote>D’Ausilio, Alessandro.<br>
“Arduino: A Low-cost Multipurpose Lab Equipment.”<br>
<i>Behavior Research Methods</i>
Online First (2011): 1–9.<br>
<a href='http://dx.doi.org/10.3758/s13428-011-0163-z'>http://dx.doi.org/10.3758/s13428-011-0163-z</a></blockquote>

<h1>Details</h1>

<h2>Python GUI (lightCueTask_demo.py)</h2>

With this example, no software interface is needed for the desired operation with the behaving mouse. For demonstration purposes, a GUI with Python was developed with just two buttons "Reward" and "Trial". The button "Reward" simulates the mouse getting the reward and the "Trial" initializes the 30sec timeout period during which the mouse has to get the reward.<br>
<br>
<img src='https://lh4.googleusercontent.com/-tpPxMLE5js4/T7PBM9KFwfI/AAAAAAAACgo/Ov8L2QbptBY/s680/lightCueTest.gif' />
<br><i>Screen capture of the <a href='http://www.geany.org/'>Geany</a> view of the code with the GUI for simulation</i>

In addition to GUI elements and Arduino initialization, from the Python end (<code>lightCueTask_demo.py</code>) only the characters to be sent as serial commands need to be specified for each button:<br>
<br>
<pre><code><br>
def buttonPress(self, widget):<br>
name = widget.get_name()<br>
if name == "reward":<br>
self.ser.write("r")<br>
elif name == "trial":<br>
self.ser.write("t")<br>
else:<br>
print "ERROR: Invalid widget name, in on_changed function"<br>
</code></pre>

<h2>Arduino sketch (lightCueTask.ino)</h2>

For the corresponding Arduino sketch, a feature of <a href='http://arduino.cc/it/Tutorial/Debounce'>debouncing</a> need to be programmed which in practice filters the digital input line from environmental noise or from spurious button presses. For additional info on how to read digital inputs see the <a href='http://arduino.cc/en/Tutorial/Switch'>Switch</a>-tutorial and how to <a href='http://quarkstream.wordpress.com/2009/12/11/arduino-3-counting-events/'>count events</a>.<br>
<br>
The Arduino sketch (<code>lightCueTask.ino</code>) now is initialized with both the current value of the digital input but also on the previous value needed to debounce digital inputs:<br>
<br>
Now initializing the trial turns ON the cue light (LED connected to the digital pin set as OUTPUT). The LED is ON (variable <code>state</code> is <code>HIGH</code>) for the timeout period of 30 seconds unless the mouse recovers the rewards changing the state to off (variable <code>state</code> is <code>LOW</code>).<br>
<br>
<pre><code><br>
//   Arduino UNO pins for DIGITAL IN and OUT<br>
//   check correspondence if you have some other board<br>
int digIn_Pin1 = 2; // to read whether the mouse has collected the reward<br>
int digIn_Pin2 = 4; // to detect when the 30 sec period starts<br>
int digOut_Pin = 13; // to switch on the cue light<br>
<br>
// Auxiliary variables adopted from the "Switch" -tutorial<br>
// http://arduino.cc/en/Tutorial/Switch<br>
<br>
int state = LOW;      // the current state of the output pin<br>
int reading_Reward;           // the current reading from the input pin 1 (REWARD)<br>
int reading_Trial;           // the current reading from the input pin 2 (TRIAL)<br>
int previous_Reward = LOW;    // the previous reading from the input pin 1 (REWARD)<br>
int previous_Trial = LOW;    // the previous reading from the input pin 2 (TRIAL)<br>
<br>
// the follow variables are long's because the time, measured in miliseconds,<br>
// will quickly become a bigger number than can be stored in an int.<br>
long time = 0;         // the last time the output pin was toggled<br>
long debounce = 200;   // the debounce time, increase if the output flickers<br>
<br>
long timeInit = 0; // used to track whether 30 sec has passed<br>
long trialTime = 30000; // 30 sec in milliseconds<br>
</code></pre>

Now the debounce time (<code>long</code> variable <code>debounce</code>) and trial timeout period (<code>long</code> variable <code>trialTime</code>) are "hard-wired" in the sketch. In practice they could be made dynamically adjustable either from computer or reading a potentiometer (analog input) or in discrete steps using switches/keyboards, but for our purposes hard-wired was sufficient.<br>
<br>
Now in the Arduino sketch in order to confirm the <b>"Init"</b> signal, the following code is needed:<br>
<br>
<pre><code><br>
if (reading_Trial == HIGH &amp;&amp; previous_Trial == LOW &amp;&amp; millis() - time &gt; debounce) {<br>
time = millis();<br>
timeInit = millis();<br>
state = HIGH;<br>
</code></pre>

Where in practice the trial have to be initialized (reading_Trial == HIGH), the previous state of the trial has to be OFF (previous_Trial == LOW), where at least the <code>debounce</code> have to have passed since last reward signal (<code>time</code>). The Arduino command <a href='http://arduino.cc/en/Reference/Millis'>millis()</a> returns the time since the Arduino program was initialized in milliseconds, and is comparable to getting current system time.<br>
<br>
If the conditions are met, the time variables <code>time</code> and <code>timeInit</code> are resetted to "current time" when the conditions are met. The variable <code>state</code> controls the LED so setting it <code>HIGH</code> will turn on the LED used as the cue light.<br>
<br>
Similarly the conditionals for "Reward" signal are checked:<br>
<br>
<pre><code><br>
if (reading_Reward == HIGH &amp;&amp; previous_Reward == LOW &amp;&amp; millis() - time &gt; debounce) {<br>
time = millis();<br>
state = LOW;<br>
}<br>
</code></pre>

Now if the reward is collected, the LED is turned off (setting <code>state</code> to <code>LOW</code>).<br>
<br>
Finally for timeout behavior of the trial one needs to check in the Arduino sketch whether 30 seconds has passed from the initialization as implemented as following:<br>
<br>
<pre><code><br>
// CHECK whether trialTime (e.g. 30 sec) has passed<br>
if (millis() - timeInit &gt; trialTime ) {<br>
state = LOW;<br>
}<br>
</code></pre>

The variable <code>timeInit</code> stores the time of when the trial was initialized and the difference between this and current time (<code>millis()</code>) is compared to the user-set threshold for trial length <code>trialTime</code>.<br>
<br>
The cue light is refreshed on every loop iteration with the following command:<br>
<br>
<pre><code><br>
// Write the digital output to the CUE LIGHT<br>
digitalWrite(digOut_Pin, state);<br>
</code></pre>

And finally the current states of the digital inputs are saved as the previous states at the end of loop for the next execution of the loop:<br>
<br>
<pre><code><br>
// Store previous values<br>
previous_Reward = reading_Reward;<br>
previous_Trial = reading_Trial;<br>
</code></pre>

<h1>Scientific References</h1>

<h2>Rodent models for cognitive research</h2>

<ul><li>Malkki, Hemi A. I., Laura A. B. Donga, Sabine E. de Groot, Francesco P. Battaglia, and Cyriel M. A. Pennartz. “Appetitive Operant Conditioning in Mice: Heritability and Dissociability of Training Stages.” Frontiers in Behavioral Neuroscience 4 (November 4, 2010). <a href='http://dx.doi.org/10.3389/fnbeh.2010.00171'>http://dx.doi.org/10.3389/fnbeh.2010.00171</a>.</li></ul>

<ul><li>Histed, Mark H, Lauren A Carvalho, and John H. R Maunsell. “Psychophysical Measurement of Contrast Sensitivity in the Behaving Mouse.” Journal of Neurophysiology 107, no. 3 (February 1, 2012): 758–765. <a href='http://dx.doi.org/10.1152/jn.00609.2011'>http://dx.doi.org/10.1152/jn.00609.2011</a>.</li></ul>

<h2>Vision research on rodents</h2>

<ul><li>Hübener, Mark. “Mouse Visual Cortex.” Current Opinion in Neurobiology 13, no. 4 (2003): 413–420. <a href='http://dx.doi.org/10.1016/S0959-4388(03)00102-8'>http://dx.doi.org/10.1016/S0959-4388(03)00102-8</a>.</li></ul>

<ul><li>Prusky, G.T., and R.M. Douglas. “Characterization of Mouse Cortical Spatial Vision.” Vision Research 44, no. 28 (December 2004): 3411–3418. <a href='http://dx.doi.org/10.1016/j.visres.2004.09.001'>http://dx.doi.org/10.1016/j.visres.2004.09.001</a>.</li></ul>

<ul><li>Jacobs, Gerald H., Gary A. Williams, and John A. Fenwick. “Influence of Cone Pigment Coexpression on Spectral Sensitivity and Color Vision in the Mouse.” Vision Research 44, no. 14 (June 2004): 1615–1622. <a href='http://dx.doi.org/10.1016/j.visres.2004.01.016'>http://dx.doi.org/10.1016/j.visres.2004.01.016</a>.</li></ul>

<ul><li>Busse, Laura, Asli Ayaz, Neel T. Dhruv, Steffen Katzner, Aman B. Saleem, Marieke L. Schölvinck, Andrew D. Zaharia, and Matteo Carandini. “The Detection of Visual Contrast in the Behaving Mouse.” The Journal of Neuroscience 31, no. 31 (August 3, 2011): 11351–11361. <a href='http://dx.doi.org/10.1523/JNEUROSCI.6689-10.2011'>http://dx.doi.org/10.1523/JNEUROSCI.6689-10.2011</a>.</li></ul>

<ul><li>Niell, Cristopher M. “Exploring the Next Frontier of Mouse Vision.” Neuron 72, no. 6 (December 22, 2011): 889–892. <a href='http://dx.doi.org/10.1016/j.neuron.2011.12.011'>http://dx.doi.org/10.1016/j.neuron.2011.12.011</a>.</li></ul>

<h2>Commercial products</h2>

<ul><li><b>MED Associates</b>, <a href='http://www.med-associates.com/'>http://www.med-associates.com/</a>, under "Operant Conditioning & General Behavior" for example providing various "nose poke" devices: <a href='http://www.med-associates.com/products-page/operant-conditioning-general-behavior/'>http://www.med-associates.com/products-page/operant-conditioning-general-behavior/</a></li></ul>

<ul><li><b>Coulbourn Instruments</b>, <a href='http://www.coulbourn.com/category_s/312.htm'>http://www.coulbourn.com/category_s/312.htm</a></li></ul>

<ul><li><b>Campden Instruments Ltd</b>, Nose poke detector: <a href='http://www.campden-inst.com/product_list.asp?SubCatID2=53'>http://www.campden-inst.com/product_list.asp?SubCatID2=53</a></li></ul>

<ul><li><b>Lafayette Instrument</b>, Nose poke devices: <a href='http://www.lafayetteneuroscience.com/product_detail.asp?ItemID=2105'>http://www.lafayetteneuroscience.com/product_detail.asp?ItemID=2105</a>