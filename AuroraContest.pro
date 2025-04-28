TEMPLATE = subdirs
CONFIG   += ordered

# Сначала сервис, затем клиент
SUBDIRS += \
    Service \
    confManagerApplication1

# Если нужно освоить пути:
Service.subdir                = Service
confManagerApplication1.subdir = confManagerApplication1
