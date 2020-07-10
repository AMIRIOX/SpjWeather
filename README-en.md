Note that this program must be used under the 64-bit Windows operating system!Both Windows 10 and Windows 8.1 were tested for compatibility and worked as long as they were 64-bit operating systems







Basic program information



\* Name: 'Special Judge Weather System'

\* Function: Weather forecast

\* development language: 'C++'

\* Development environment: 'Visual Studio 2019' (partially using 'VSCode'), 'Qt5.12.1'

\* Operating environment: ** must be a 64-bit operating system **, in addition, the 'MSVC' runtime environment has been integrated

\* By Amy Riox



\# Program fundamentals/application techniques



\1. 'QtNetworkAccessManager' sends a GET request to the API

\2. 'QJsonArray', 'QJsonObject' to process the returned Json file

\3. Initialization interface: get the supported locale information from 'city.json' and add it to the drop-down menu (' QComboBox ')

\4. Use 'QTimer' for timing. **API requires at least 3s** between two requests



\# Precautions



\1. The installation process is similar to the ordinary software installation process. Please see the video for details, which will not be repeated here

\2. ** the software is developed in the environment of Windows10-64bit and debugged in the environment of Windows8.1. ** it can run normally

\3. The 'setting' at the bottom of the main interface is clickable. After clicking, select the city for configuration (** Default Beijing **).

\4. Please do not set and save too often.API requirements should not be too frequent access, although the use of the QTimer timing limit, but still hope not too frequent **

\5. Administrative centres comprising 34 provincial districts, as well as other cities to be added by the author as appropriate