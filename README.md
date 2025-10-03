# VideoOSD Project

Цей проєкт відображає текст на відео з можливістю налаштування через веб-інтерфейс.

Встановлення залежностей

На Linux потрібно встановити всі залежності для C++ та Python:

Оновіть пакети:

sudo apt update


Встановіть C++ залежності:

```
sudo apt install -y build-essential cmake libopencv-dev nlohmann-json3-dev
sudo apt install -y python3 python3-pip
pip3 install flask psutil
```

Збірка C++ програми

```
mkdir build
cd build
cmake ..
make
sudo make install
```

Налаштування config.json

```
Приклад config.json:
{
  "resolution": [600, 480],
  "text": "Hello OSD",
  "color": [200, 255, 255],
  "speed": 1.0,
  "args": ["/dev/video0", "-cr"]
}
```

resolution — роздільна здатність відео
text — текст для OSD
color — колір тексту та компаса у форматі RGB
speed — швидкість руху стрілки
args — аргументи запуску (камера, -cr для реверсу)

Запуск програм (веб і с++)
```
python3 app.py
```

Відкрийте у браузері: http://127.0.0.1:5000/

Логін: admin

Налаштування відео доступні на сторінці http://127.0.0.1:5000/settings

При зміні параметрів через веб-інтерфейс C++ процес перезапускається автоматично.


Запуск C++ вручну
```
main --camera /dev/video0 -cr
```

--camera — пристрій камери

-cr — реверс руху стрілки
