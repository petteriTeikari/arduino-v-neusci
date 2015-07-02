function varargout = GuiArduinoLED(varargin)

    % NOTE: This .m file is meant to be used with the adiosrv.pde Arduino
    % Sketch that is available in the Arduino I/O package available here:
    % http://www.mathworks.com/matlabcentral/fileexchange/32374
    
        % see also:
        % http://www.mathworks.com/academia/arduino-software/arduino-matlab.html
    
    % So UPLOAD FIRST that sketch to your Arduino Board before running this
    % demo

    % Begin initialization code - DO NOT EDIT
    gui_Singleton = 1;
    gui_State = struct('gui_Name',       mfilename, ...
                       'gui_Singleton',  gui_Singleton, ...
                       'gui_OpeningFcn', @GuiArduinoLED_OpeningFcn, ...
                       'gui_OutputFcn',  @GuiArduinoLED_OutputFcn, ...
                       'gui_LayoutFcn',  [] , ...
                       'gui_Callback',   []);
    if nargin && ischar(varargin{1})
        gui_State.gui_Callback = str2func(varargin{1});
    end

    if nargout
        [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
    else
        gui_mainfcn(gui_State, varargin{:});
    end
    % End initialization code - DO NOT EDIT

% --- Executes just before GuiArduinoLED is made visible.
function GuiArduinoLED_OpeningFcn(hObject, eventdata, handles, varargin)

    % Choose default command line output for GuiArduinoLED
    handles.output = hObject;

    % set default GUI ettings
    handles = setDefaultGUIsettings(handles);
    
    % initialize Arduino
    port = '/dev/tty.usbmodemfa131';
    handles.a = init_Arduino(handles, port);
  
    dispDebug = handles.a
    
    % Update handles structure
    guidata(hObject, handles);
  

    % --- Outputs from this function are returned to the command line.
    function varargout = GuiArduinoLED_OutputFcn(hObject, eventdata, handles) 

        % Get default command line output from handles structure
        varargout{1} = handles.output;

    
%% GUI OBJECTS

        % --- Executes on slider movement.
        function slider_Callback(hObject, eventdata, handles)

            sliderValue = get(hObject,'Value'); %get the string for the editText component        
            % sliderValue = str2double(sliderValue); %convert from string to number if possible, otherwise returns empty        
            sliderValue = filterSliderValue(sliderValue, handles, handles.settings, 'pwm'); % use subfunction to determine what is done for the data

            % update to GUI        
            set(handles.slider, 'Value',  sliderValue);
            set(handles.valueBox, 'String', num2str(sliderValue));

            % save to handles.values
            handles.values.pwm = sliderValue;

            % Update handles structure
            guidata(hObject, handles);

            % --- Executes during object creation, after setting all properties.
            function slider_CreateFcn(hObject, eventdata, handles)
                if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
                    set(hObject,'BackgroundColor',[.9 .9 .9]);
                end


        function valueBox_Callback(hObject, eventdata, handles)

            % Conversion from String to Double with the check of numeric value
            sliderValue = round(str2double(get(hObject,'string')));  
            sliderValue = updateSliderFromBox(sliderValue, 'pwm', handles);

            % update to GUI        
            set(handles.slider, 'Value',  sliderValue);
            set(handles.valueBox, 'String', num2str(sliderValue));

            % save to handles.values
            handles.values.pwm = sliderValue;

            guidata(hObject, handles); %updates the handles structure


            % --- Executes during object creation, after setting all properties.
            function valueBox_CreateFcn(hObject, eventdata, handles)
                if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
                    set(hObject,'BackgroundColor','white');
                end




%% GUI SUBFUNCTIONS

        % Filter the "raw filter value"
        function sliderValue = filterSliderValue(sliderValueIn, handles, settings, sliderType)

            sliderValue = floor(sliderValueIn);
            disp(['sliderValue RAW = ', num2str(sliderValueIn,3), ', OUT = ', num2str(sliderValue)])

            % you could force the value to some values if you want
            % (optional)
            
            % write the value to the Arduino
            handles.a.analogWrite(9,sliderValue);

        function value = updateSliderFromBox(value, sliderType, handles)

            % if the input is not numeric
            if numericError(value) == 1
                return
            end

            % filter the slide value for allowed values
            value = filterSliderValue(value, handles, handles.settings, sliderType);

        % checks whether the input for editText-fields is positive and numeric
        function numericErrorTrue = numericError(input)   

            numericErrorTrue = 0;

            if isnan(input)
                errordlg('You must enter a numeric value','Bad Input','modal')
                numericErrorTrue = 1;          
                return
            end

            if input < 0
                errordlg('You must enter a positive value','Bad Input','modal')
                numericErrorTrue = 1;                     
                return
            end
            
            
    % Initializes Arduino object
    function a = init_Arduino(handles, port)

    %% modified from blink_challenge.m
    
        % The "blink_challenge" is described in the last part of the Ladyada Arduino 
        % tutorial, http://www.ladyada.net/learn/arduino/ and it consists in designing 
        % a circuit (and programming the board) so that the resulting device has 5 
        % LEDs (connected to the digital outputs 9 to 13) and 4 modes (the user can 
        % switch among them using a button connected to digital input #2)

    %% the init part    
    
        % create arduino object and connect to board
        if exist('a') && isa(a,'arduino') && isvalid(a),
            % nothing to do    
        else
            try
               a=arduino(port);
            catch
               delete(instrfind({'Port'},{'/dev/ttyS101'})) 
               a=arduino(port);
            end
        end

        % initialize pins
        disp('Initializing Pins ...');
        
        % sets digital and analog (pwm) output pins   
        a.pinMode(9, 'OUTPUT'); % pwm available here
        
    function handles = setDefaultGUIsettings(handles)

        % SET DEFAULT VALUES       
        handles.settings.Min_pwm = 0;
        handles.settings.Max_pwm = 255;
            handles.settings.SliderStep_pwm = [0.01 0.1];

        handles.settings.Min_freqPWM = 0;
        handles.settings.Max_freqPWM = 1000;
            handles.settings.SliderStep_freqPWM = [10 100];

        handles.settings.Min_freqLight = 0;
        handles.settings.Max_freqLight = 5;
            handles.settings.SliderStep_freqLight = [0.1 0.5];

        % PWM SLIDERS

            set(handles.slider, 'SliderStep', handles.settings.SliderStep_pwm);
            set(handles.slider, 'Min',        handles.settings.Min_pwm);
            set(handles.slider, 'Max',        handles.settings.Max_pwm); 

