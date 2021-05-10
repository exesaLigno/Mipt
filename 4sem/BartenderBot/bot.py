#!/usr/bin/python3
import TelegramAPI
import sys
import json
import time
import threading
import Bartender
from context import Context
from subprocess import Popen, PIPE



DEBUG = True



commands_list = {
                    "start": "Начало работы с чат-ботом и показ справки",
                    "help": "Показ справки",
                    "bar": "Открывает персональный бар",
                    "settings": "Настройки чат-бота",
                    "fortune": "Плюется случайной шуткой"
                }

with open("static/pages.json") as static_data:
    pages = json.loads(static_data.read())


bot = TelegramAPI.Bot(token_file = "config.json")
bot.setCommandsList(commands_list)

bartender = Bartender.BarTender()
bartender.setPath("/home/exesa_ligno/Documents/Study/Programming/Mipt/4sem/BartenderBot")
bartender.loadReceipes()
bartender.loadBars()


@bot.message_handler()
def handler(message):
    if message.text.startswith("/"):
        command_processor(message)
    else:
        search_processor(message)


@bot.callback_handler()
def callback_handler(callback):

    answer = "Выполнено"

    c = Context.getContext(callback.message)
    bar = bartender.getBar(callback.chat_id)

    if c == None:
        callback.answer("Похоже, что произошла внутренняя ошибка, сообщение будет удалено. Ваши данные не будут утеряны.", show_alert = True)
        callback.message.delete()
        return

    if callback.data == "back":
        c.back()

    elif callback.data == "close":
        c.close()
        callback.message.delete()
        return

    elif callback.data == "prev":
        context = c.getPageInfo().split(":")
        context[1] = str(int(context[1]) - 1)
        c.replaceContext(":".join(context))

    elif callback.data == "change_pages_ignore":
        callback.answer("Листать дальше некуда")
        return

    elif callback.data == "next":
        context = c.getPageInfo().split(":")
        context[1] = str(int(context[1]) + 1)
        c.replaceContext(":".join(context))

    elif callback.data == "update_shoplist":
        context = Context.getContext(callback.message)
        cocktail_id = int(context.getPageInfo().split(":").pop())
        cocktail = bartender.getCocktail(cocktail_id)
        bar.addMissingToShoplist(cocktail)
        answer = "Недостающие ингредиенты добавлены в ваш шоплист"

    elif callback.data == "update_barlist":
        context = Context.getContext(callback.message)
        cocktail_id = int(context.getPageInfo().split(":").pop())
        cocktail = bartender.getCocktail(cocktail_id)
        bar.addMissingToBar(cocktail)
        answer = "Недостающие ингредиенты добавлены в ваш бар"

    elif callback.data == "add_barlist":
        ingredient = c.getPageInfo().split(":").pop()
        if ingredient not in bar.bar_list:
            bar.bar_list.append(ingredient)
        if ingredient in bar.shoplist:
            bar.shoplist.remove(ingredient)
        bar.dumpBar()
        answer = "Добавлено в бар"

    elif callback.data == "add_shoplist":
        ingredient = c.getPageInfo().split(":").pop()
        if ingredient not in bar.shoplist:
            bar.shoplist.append(ingredient)
        if ingredient in bar.bar_list:
            bar.bar_list.remove(ingredient)
        bar.dumpBar()
        answer = "Добавлено в шоплист"

    elif callback.data == "buyed":
        ingredient = c.getPageInfo().split(":").pop()
        if ingredient in bar.shoplist:
            bar.shoplist.remove(ingredient)
        if ingredient not in bar.bar_list:
            bar.bar_list.append(ingredient)
        bar.dumpBar()
        answer = "Отмечено как купленное"

    elif callback.data == "del_barlist":
        ingredient = c.getPageInfo().split(":").pop()
        if ingredient in bar.bar_list:
            bar.bar_list.remove(ingredient)
        bar.dumpBar()
        answer = "Удалено из бара"

    elif callback.data == "del_shoplist":
        ingredient = c.getPageInfo().split(":").pop()
        if ingredient in bar.shoplist:
            bar.shoplist.remove(ingredient)
        bar.dumpBar()
        answer = "Удалено из шоплиста"

    elif callback.data == "switch_favourites":
        cocktail = int(c.getPageInfo().split(":").pop())
        if cocktail not in bar.favourites_list:
            bar.favourites_list.append(cocktail)
            answer = "Коктейль добавлен в избранное"
        else:
            bar.favourites_list.remove(cocktail)
            answer = "Коктейль удалён из избранного"
        bar.dumpBar()

    else:
        c.addContext(callback.data)

    if DEBUG:
        print(c.context)

    text = getPageText(c.getPageInfo(), callback.chat_id)
    keyboard = getPageKeyboard(c.getPageInfo(), callback.chat_id)

    sended = callback.message.edit(text, reply_markup = keyboard)

    callback.answer(answer)



def command_processor(message):
    command = message.text[1::]

    text = getPageText(command, message.chat_id)
    keyboard = getPageKeyboard(command, message.chat_id)

    sended = message.answer(text, reply_markup = keyboard)

    c = Context(sended)
    c.addContext(command)

    message.delete()


def search_processor(message):
    search_results = message.answer("Окей, ищу коктейль <b>" + message.text + "</b>")

    c = Context(search_results)
    c.addContext("search:1:" + message.text)

    text = getPageText(c.getPageInfo(), message.chat_id)
    keyboard = getPageKeyboard(c.getPageInfo(), message.chat_id)

    search_results.edit(text, reply_markup = keyboard)

    message.delete()


def getPageText(context, id):

    pages_count = lambda x: x // 7 + (0 if x % 7 == 0 else 1)
    bar = bartender.getBar(id)

    text = ""

    if context == "bar":
        text += "<b>Добро пожаловать в ваш бар!</b>\n\nЗдесь вы можете просмотреть, добавить или удалить имеющиеся у вас напитки в разделе <i>Мой бар</i>, свериться со списком покупок в разделе <i>Шоплист</i>"

    elif context in ["start", "help", "settings"]:
        text += "Служебная страница"

    elif context == "fortune":
        fortune = Popen("fortune", shell = True, stdout = PIPE, stderr = PIPE)
        fortune.wait()
        result = fortune.communicate()

        if fortune.returncode != 0:
            print("error: " + result[1].decode("UTF-8"))
            text += "Кажется, что-то пошло не так"
        else:
            text += "<code>" + result[0].decode("UTF-8") + "</code>"

    elif context.startswith("my_bar"):
        page = context.split(":").pop()
        results_count = len(bar.bar_list)

        if results_count != 0:
            text += "Вот какие напитки у вас имеются"
            if pages_count(results_count) > 1:
                text += "\n<code>Страница {} из {}</code>".format(page, pages_count(results_count))
        else:
            text += "Пока что у вас нет ни одного ингредиента в баре. Вы можете их добавить вручную или через страницу рецепта коктейля."

    elif context.startswith("shoplist"):
        page = context.split(":").pop()
        results_count = len(bar.shoplist)

        if results_count != 0:
            text += "Вот ваш список покупок"
            if pages_count(results_count) > 1:
                text += "\n<code>Страница {} из {}</code>".format(page, pages_count(results_count))
        else:
            text += "Пока что ваш список покупок пуст. Вы можете добавить ингредиенты вручную или через страницу рецепта коктейля."

    elif context.startswith("favourites"):
        page = context.split(":").pop()
        results_count = len(bar.favourites_list)

        if results_count != 0:
            text += "Вот ваши избранные рецепты"
            if pages_count(results_count) > 1:
                text += "\n<code>Страница {} из {}</code>".format(page, pages_count(results_count))
        else:
            text += "Вы пока что ничего не добавили в список избранных рецептов. Вы можете сделать это через страницу рецепта коктейля."

    elif context.startswith("all"):
        page = context.split(":").pop()
        results_count = len(bartender.receipes_list)

        text += "Вот все рецепты, имеющиеся в чат-боте"
        if pages_count(results_count) > 1:
            text += "\n<code>Страница {} из {}</code>".format(page, pages_count(results_count))

    elif context.startswith("search"):
        splitted = context.split(":")
        request = splitted.pop()
        page = splitted.pop()
        results = bartender.search(request)
        cocktails_count = len(results["cocktails_list"])
        ingredients_count = len(results["ingredients_list"])

        if cocktails_count != 0 and ingredients_count != 0:
            text += "По запросу <b>" + request + "</b> были найдены рецепты коктейлей и ингредиенты. Для того, чтобы выбрать, какие результаты поиска вы хотите посмотреть, нажмите на одну из кнопок ниже."
        elif cocktails_count != 0 and ingredients_count == 0:
            text += "Вот, что мне удалось найти по запросу <b>" + request + "</b>"
            if pages_count(cocktails_count) > 1:
                text += "\n<code>Страница {} из {}</code>".format(page, pages_count(cocktails_count))
        elif cocktails_count == 0 and ingredients_count != 0:
            text += "Вот, что мне удалось найти по запросу <b>" + request + "</b>"
            if pages_count(ingredients_count) > 1:
                text += "\n<code>Страница {} из {}</code>".format(page, pages_count(ingredients_count))
        else:
            text += "К сожалению, по запросу <b>" + request + "</b> ничего не удалось найти"

    elif context.startswith("csearch"):
        splitted = context.split(":")
        request = splitted.pop()
        page = splitted.pop()
        results = bartender.search(request)
        cocktails_count = len(results["cocktails_list"])

        text += "Вот, что мне удалось найти по запросу <b>" + request + "</b> среди коктейлей"
        if pages_count(cocktails_count) > 1:
            text += "\n<code>Страница {} из {}</code>".format(page, pages_count(cocktails_count))

    elif context.startswith("isearch"):
        splitted = context.split(":")
        request = splitted.pop()
        page = splitted.pop()
        results = bartender.search(request)
        ingredients_count = len(results["ingredients_list"])

        text += "Вот, что мне удалось найти по запросу <b>" + request + "</b> среди ингредиентов"
        if pages_count(ingredients_count) > 1:
            text += "\n<code>Страница {} из {}</code>".format(page, pages_count(ingredients_count))

    elif context.startswith("cocktail"):
        cocktail_id = int(context.split(":").pop())

        text += "Вот рецепт коктейля <b>" + bartender.getCocktail(cocktail_id).name + "</b>\n\nДля приготовления понадобятся:\n"
        missing_count = 0
        for ingredient in bartender.getCocktail(cocktail_id).ingredients:
            if ingredient in bar.bar_list:
                text += "  ▣ "
            else:
                text += "  □ "
                missing_count += 1
            text += "<i>" + ingredient + "</i>\n"

        text += "\n"
        text += bartender.getCocktail(cocktail_id).receipe

        if missing_count != 0:
            text += "\n\n<code>Некоторых ингредиентов для этого коктейля не хватает, но вы можете добавить их в шоплист, нажав соответствующую кнопку ниже</code>\n"
        else:
            text += "\n\n<code>У вас есть все необходимое для приготовления этого рецепта!</code>\n"

    elif context.startswith("ingredient"):
        ingredient_name = context.split(":").pop()
        text += ingredient_name

    else:
        text += "Похоже, что-то пошло не так, рекомендуем <b>закрыть</b> данное сообщение. Если вы снова увидите это сообщение, рекомендуем сообщить администраторам о проблеме."

    return text


def getPageKeyboard(context, id):

    pages_count = lambda x: x // 7 + (0 if x % 7 == 0 else 1)
    bar = bartender.getBar(id)

    if context == "fortune":
        return [[{"text": "Отличная шутка 👍", "callback_data": "close"}]]

    if context == "bar":
        return [[{"text": "Мой бар", "callback_data": "my_bar:1"}, {"text": "Шоплист", "callback_data": "shoplist:1"}, {"text": "Избранное", "callback_data": "favourites:1"}],
                [{"text": "Доступные рецепты", "callback_data": "available:1"}, {"text": "Все рецепты", "callback_data": "all:1"}],
                [{"text": "❌ Закрыть", "callback_data": "close"}]]

    elif context.startswith("my_bar"):
        splitted = context.split(":")
        page = int(splitted.pop())
        results = bar.bar_list
        keyboard = [[{"text": ingredient, "callback_data": "ingredient:" + ingredient}] for ingredient in results[7 * (page - 1) : 7 * page]]
        if pages_count(len(results)) > 1:
            if page == 1:
                page_change_buttons = [{"text": "🚫", "callback_data": "change_pages_ignore"}, {"text": ">>", "callback_data": "next"}]
            elif page == pages_count(len(results)):
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": "🚫", "callback_data": "change_pages_ignore"}]
            else:
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": ">>", "callback_data": "next"}]
            keyboard += [page_change_buttons]
        keyboard += [[{"text": "⬅️ Назад", "callback_data": "back"}]]

        return keyboard

    elif context.startswith("shoplist"):
        splitted = context.split(":")
        page = int(splitted.pop())
        results = bar.shoplist
        keyboard = [[{"text": ingredient, "callback_data": "ingredient:" + ingredient}] for ingredient in results[7 * (page - 1) : 7 * page]]
        if pages_count(len(results)) > 1:
            if page == 1:
                page_change_buttons = [{"text": "🚫", "callback_data": "change_pages_ignore"}, {"text": ">>", "callback_data": "next"}]
            elif page == pages_count(len(results)):
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": "🚫", "callback_data": "change_pages_ignore"}]
            else:
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": ">>", "callback_data": "next"}]
            keyboard += [page_change_buttons]
        keyboard += [[{"text": "⬅️ Назад", "callback_data": "back"}]]

        return keyboard

    elif context.startswith("favourites"):
        splitted = context.split(":")
        page = int(splitted.pop())
        results = bar.favourites_list
        keyboard = [[{"text": bartender.getCocktail(id).name + " ⭐️", "callback_data": "cocktail:" + str(id)}] for id in results[7 * (page - 1) : 7 * page]]
        if pages_count(len(results)) > 1:
            if page == 1:
                page_change_buttons = [{"text": "🚫", "callback_data": "change_pages_ignore"}, {"text": ">>", "callback_data": "next"}]
            elif page == pages_count(len(results)):
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": "🚫", "callback_data": "change_pages_ignore"}]
            else:
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": ">>", "callback_data": "next"}]
            keyboard += [page_change_buttons]
        keyboard += [[{"text": "⬅️ Назад", "callback_data": "back"}]]

        return keyboard

    elif context.startswith("available"):
        splitted = context.split(":")
        page = int(splitted.pop())
        results = bartender.getCocktailsByIngredients(bar.bar_list)

        for cocktail in bar.favourites_list:
            if cocktail in results:
                results.remove(cocktail)
                results.insert(0, cocktail)

        keyboard = [[{"text": bartender.getCocktail(id).name + (" ⭐️" if id in bar.favourites_list else ""), "callback_data": "cocktail:" + str(id)}] for id in results[7 * (page - 1) : 7 * page]]
        if pages_count(len(results)) > 1:
            if page == 1:
                page_change_buttons = [{"text": "🚫", "callback_data": "change_pages_ignore"}, {"text": ">>", "callback_data": "next"}]
            elif page == pages_count(len(results)):
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": "🚫", "callback_data": "change_pages_ignore"}]
            else:
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": ">>", "callback_data": "next"}]
            keyboard += [page_change_buttons]
        keyboard += [[{"text": "⬅️ Назад", "callback_data": "back"}]]

        return keyboard

    elif context.startswith("all"):
        splitted = context.split(":")
        page = int(splitted.pop())
        results = bartender.receipes_list

        keyboard = [[{"text": cocktail.name + (" ⭐️" if cocktail.id in bar.favourites_list else ""), "callback_data": "cocktail:" + str(cocktail.id)}] for cocktail in results[7 * (page - 1) : 7 * page]]
        if pages_count(len(results)) > 1:
            if page == 1:
                page_change_buttons = [{"text": "🚫", "callback_data": "change_pages_ignore"}, {"text": ">>", "callback_data": "next"}]
            elif page == pages_count(len(results)):
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": "🚫", "callback_data": "change_pages_ignore"}]
            else:
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": ">>", "callback_data": "next"}]
            keyboard += [page_change_buttons]
        keyboard += [[{"text": "⬅️ Назад", "callback_data": "back"}]]

        return keyboard

    elif context.startswith("ingredient"):
        ingredient_name = context.split(":").pop()
        keyboard = []
        if ingredient_name not in bar.bar_list and ingredient_name not in bar.shoplist:
            keyboard += [[{"text": "Добавить в бар", "callback_data": "add_barlist"}],
                        [{"text": "Добавить в шоплист", "callback_data": "add_shoplist"}]]
        elif ingredient_name not in bar.bar_list and ingredient_name in bar.shoplist:
            keyboard += [[{"text": "Куплено", "callback_data": "buyed"}],
                        [{"text": "Удалить из шоплиста", "callback_data": "del_shoplist"}]]
        elif ingredient_name in bar.bar_list and ingredient_name not in bar.shoplist:
            keyboard += [[{"text": "Удалить из бара", "callback_data": "del_barlist"}]]
        keyboard += [[{"text": "⬅️ Назад", "callback_data": "back"}]]
        return keyboard

    elif context.startswith("search"):
        splitted = context.split(":")
        request = splitted.pop()
        page = int(splitted.pop())
        results = bartender.search(request)

        if len(results["cocktails_list"]) == 0 and len(results["ingredients_list"]) != 0:
            results = results["ingredients_list"]
            keyboard = [[{"text": ingredient, "callback_data": "ingredient:" + ingredient}] for ingredient in results[7 * (page - 1) : 7 * page]]
        elif len(results["cocktails_list"]) != 0 and len(results["ingredients_list"]) == 0:
            results = results["cocktails_list"]
            keyboard = [[{"text": bartender.getCocktail(id).name + (" ⭐️" if id in bar.favourites_list else ""), "callback_data": "cocktail:" + str(id)}] for id in results[7 * (page - 1) : 7 * page]]
        elif len(results["cocktails_list"]) != 0 and len(results["ingredients_list"]) != 0:
            keyboard = [[{"text": "Коктейли", "callback_data": "csearch:1:" + request}, {"text": "Ингредиенты", "callback_data": "isearch:1:" + request}],
                        [{"text": "❌ Закрыть", "callback_data": "close"}]]
            return keyboard
        else:
            keyboard = [[{"text": "❌ Закрыть", "callback_data": "close"}]]
            return keyboard

        if pages_count(len(results)) > 1:
            if page == 1:
                page_change_buttons = [{"text": "🚫", "callback_data": "change_pages_ignore"}, {"text": ">>", "callback_data": "next"}]
            elif page == pages_count(len(results)):
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": "🚫", "callback_data": "change_pages_ignore"}]
            else:
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": ">>", "callback_data": "next"}]
            keyboard += [page_change_buttons]
        keyboard += [[{"text": "❌ Закрыть", "callback_data": "close"}]]

        return keyboard

    elif context.startswith("csearch"):
        splitted = context.split(":")
        request = splitted.pop()
        page = int(splitted.pop())
        results = bartender.search(request)["cocktails_list"]

        keyboard = [[{"text": bartender.getCocktail(id).name + (" ⭐️" if id in bar.favourites_list else ""), "callback_data": "cocktail:" + str(id)}] for id in results[7 * (page - 1) : 7 * page]]
        if pages_count(len(results)) > 1:
            if page == 1:
                page_change_buttons = [{"text": "🚫", "callback_data": "change_pages_ignore"}, {"text": ">>", "callback_data": "next"}]
            elif page == pages_count(len(results)):
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": "🚫", "callback_data": "change_pages_ignore"}]
            else:
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": ">>", "callback_data": "next"}]
            keyboard += [page_change_buttons]
        keyboard += [[{"text": "⬅️ Назад", "callback_data": "back"}]]

        return keyboard

    elif context.startswith("isearch"):
        splitted = context.split(":")
        request = splitted.pop()
        page = int(splitted.pop())
        results = bartender.search(request)["ingredients_list"]

        keyboard = [[{"text": ingredient, "callback_data": "ingredient:" + ingredient}] for ingredient in results[7 * (page - 1) : 7 * page]]
        if pages_count(len(results)) > 1:
            if page == 1:
                page_change_buttons = [{"text": "🚫", "callback_data": "change_pages_ignore"}, {"text": ">>", "callback_data": "next"}]
            elif page == pages_count(len(results)):
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": "🚫", "callback_data": "change_pages_ignore"}]
            else:
                page_change_buttons = [{"text": "<<", "callback_data": "prev"}, {"text": ">>", "callback_data": "next"}]
            keyboard += [page_change_buttons]
        keyboard += [[{"text": "⬅️ Назад", "callback_data": "back"}]]

        return keyboard

    elif context.startswith("cocktail"):
        cocktail_id = int(context.split(":").pop())

        missing_count = 0
        for ingredient in bartender.getCocktail(cocktail_id).ingredients:
            if ingredient not in bar.bar_list:
                missing_count += 1

        keyboard = []

        keyboard += [[{"text": "Добавить в избранное" if cocktail_id not in bar.favourites_list else "Удалить из избранного", "callback_data": "switch_favourites"}]]

        if missing_count != 0:
            keyboard += [[{"text": "Добавить недостающее в шоплист", "callback_data": "update_shoplist"}],
                            [{"text": "У меня есть все ингредиенты", "callback_data": "update_barlist"}]]

        keyboard += [[{"text": "⬅️ К результатам поиска", "callback_data": "back"}]]

        return keyboard

    else:
        return [[{"text": "❌ Закрыть", "callback_data": "close"}]]



if __name__ == "__main__":
    polling_thread = threading.Thread(target = bot.polling, daemon = True)
    polling_thread.start()

    while True:
        pass
