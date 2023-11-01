add_library(Botdefs)

target_link_libraries(
    Botdefs
    PRIVATE Qt::Core Qt::Quick
)

target_sources(
    Botdefs PRIVATE
    bot.h bot.cpp

    Bots

    aibot.h aibot.cpp
    eating_bot.h eating_bot.cpp
    followmouse_bot.h followmouse_bot.cpp
    killing_bot.h killing_bot.cpp
    stupid_bot.h stupid_bot.cpp

    neuralnet.h neuralnet.cpp
)

