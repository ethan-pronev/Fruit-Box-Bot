## Setup

- Install [Tesseract](https://github.com/UB-Mannheim/tesseract/wiki)
  - If you encounter errors when running you may have to download [this file](https://github.com/tesseract-ocr/tessdata/blob/main/eng.traineddata) and place it in ```/PATH/TO/TESSERACT/INSTALL/tessdata``` (or replace the existing file of the same name)
- Install all python libraries used in ```main.py```

```pip install ...```
- Create the C++ executable:

```g++ algorithm.cpp -o algorithm.exe```
- Open [Fruit Box](https://en.gamesaien.com/game/fruit_box/) in your browser
- In ```main.py``` set the pixel varibles to match your screen
- Run the python program and quickly switch to your browser window

```python main.py```
- While the bot is running, press ```Esc``` at any time to stop it