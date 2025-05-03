## Описание

1. Реализован DBus-сервис с именем `com.system.configurationManager` на сессионной шине, который:

   - при запуске считывает файлы конфигураций приложений из папки `~/com.system.configurationManager/` и  
     создаёт для каждого файла конфигурации D-Bus-объект  
     `com.system.configurationManager.Application.{applicationName}` с интерфейсом  
     `com.system.configurationManager.Application.Configuration`;  
   - предоставляет в этом интерфейсе методы:  
     - `void ChangeConfiguration(const QString &key, const QDBusVariant &value)` — изменяет  
       указанный параметр в конфиге и выдаёт D-Bus-ошибку в случае неудачи;  
     - `QVariantMap GetConfiguration()` — возвращает полную карту конфигурации приложения;  
   - при изменении любой настройки эмитит сигнал  
     `configurationChanged(QVariantMap conf)`, где `conf` имеет D-Bus-тип `a{sv}`.

2. Реализовано консольное приложение `confManagerApplication1`, которое:

   - подписывается на сигнал `configurationChanged` на объекте  
     `com.system.configurationManager.Application.confManagerApplication1` сервиса  
     `com.system.configurationManager` и применяет новые параметры в случае их изменения;  
   - хранит свой конфиг в файле  
     `~/com.system.configurationManager/confManagerApplication1.json` с полями:  
     1. `"Timeout"` (uint, миллисекунды) — интервал между выводами строки;  
     2. `"TimeoutPhrase"` (string) — фраза, выводимая каждые `Timeout` мс.

---

## Сборка проекта с помощью qmake

### 1. Сборка через общий Subdirs-проект

```bash
cd path/to/AuroraContest
qmake ConfigurationManagerProject.pro
make
```

### 2. Сборка отдельных подпроектов

#### Сервис

```bash
cd path/to/AuroraContest/Service
qmake service.pro
make
```

#### Приложение

```bash
cd path/to/AuroraContest/confManagerApplication1
qmake confManagerApplication1.pro
make
```

---

## Запуск и тестирование

1. **Запуск сервиса**
   В терминале:

   ```bash
   ./Service/configurationManagerService
   ```

   (либо настроить автозапуск через D-Bus .service-файл)

2. **Запуск клиента**
   В другом терминале:

   ```bash
   ./confManagerApplication1/confManagerApplication1
   ```

   Клиент сразу прочитает начальную конфигурацию и начнёт каждую `Timeout` мс выводить строку `TimeoutPhrase`.

3. **Изменение конфигурации через gdbus**
   Настройка .service-файла для авто-активации

    Чтобы gdbus call сам запускал сервис при первом обращении, создайте файл:
    
    ```bash# ~/.local/share/dbus-1/services/com.system.configurationManager.service
    [D-BUS Service]
    Name=com.system.configurationManager
    Exec=/полный/путь/до/Service/configurationManagerService
    ```

    Выполните команду для изменения конфигурации:
   ```bash
   gdbus call --session \
     --dest com.system.configurationManager \
     --object-path /com/system/configurationManager/Application/confManagerApplication1 \
     --method com.system.configurationManager.Application.Configuration.ChangeConfiguration \
     s "TimeoutPhrase" '<"Please stop me">'
   ```

   После этого клиент в реальном времени получит сигнал `configurationChanged`, перезапустит таймер
   и начнёт выводить новую фразу.
   
## Программные требования

- **Компилятор**: GCC ≥ 5.0 или Clang с поддержкой C++11.
- **Qt**: версия ≥ 5.9, модули  
  - QtCore  
  - QtDBus  
  - QtNetwork (если планируется расширение)  
  - QtConcurrent (опционально, для фоновых задач)  
- **Инструменты сборки**:  
  - qmake (часть Qt)  
  - make (или другой `make`-совместимый билд-системы)  
- **Пакет для работы с D-Bus из командной строки**:  
  - `glib2-utils` (утилита `gdbus`)  

---


