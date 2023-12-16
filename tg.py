import requests
from telegram import Bot
from telegram import InputFile

BOT_TOKEN = YOUR_TOKEN
CHAT_ID = YOUR_CHAT_ID

def text(message):
    bot_token = BOT_TOKEN
    chat_id = CHAT_ID
    api_url = f"https://api.telegram.org/bot{bot_token}/sendMessage"
    params = {
        'chat_id': chat_id,
        'text': message,
    }
    response = requests.post(api_url, params=params)
    result = response.json()
    
def video(video_path,caption_text):
    bot = Bot(token=BOT_TOKEN)
    chat_id = CHAT_ID
    video_message = bot.send_video(chat_id=chat_id, video=InputFile(open(video_path, 'rb')),caption=caption_text)
    return True if video_message.video else False
