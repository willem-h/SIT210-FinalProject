import requests
import time

from flask import Flask
from gpiozero import TonalBuzzer
from gpiozero.tones import Tone

api = Flask(__name__)
buzzer = TonalBuzzer(13)

notify_url = 'https://maker.ifttt.com/trigger/driveway_notification/with/key/<IFTTT API KEY>'

def ring_doorbell(bell=buzzer, notes=['F4', 'B4', 'F4', 'B4']):
  for note in notes:
    bell.play(note)
    time.sleep(0.3)
  bell.stop()

def notify_phone():
  requests.get(notify_url)

@api.route('/laser-tripped')
def laser_tripped():
  ring_doorbell()
  notify_phone()
  return 'Ok', 200

if __name__ == '__main__':
  api.run(debug=True, port=80, host='0.0.0.0')

