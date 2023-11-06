import config from "common/configs/config.json" assert { type: "json" };

import compress from "@fastify/compress";

async function register(app, options)
{
    /*----------------------------------COMPRESS----------------------------------*/
    await app.register(compress);


    /*----------------------------------ERROR----------------------------------*/
    function get_default_error_title(code)
    {
        switch(code)
        {
            case 200: return "Действие было выполнено успешно!";

            case 400: return "Запрос неверен!";
            case 401: return "Ошибка авторизации!";
            case 402: return "Запрошенный ресурс доступен только после оплаты!";
            case 403: return "У вас нет доступа к запрошенному ресурсу!";
            case 404: return "Запрошенный ресурс не найден!";
            case 429: return "Вы отправляете слишком много запросов!";

            case 500: return "Ошибка сервера!";
            case 502: return "Ошибка взаимодействия!";
            case 503: return "Сервер перегружен!";
            case 504: return "Ошибка взаимодействия!";
            case 508: return "Ошибка обработки запроса!";

            default: return "Произошла неизвестная ошибка!";
        }
    }
    function get_default_error_message(code)
    {
        switch(code)
        {
            case 200: return "Вы будете перенаправлены обратно в ближайшее время!";

            case 400: return "Повторите попытку!";
            case 401: return "Авторизуйтесь и повторите попытку!";
            case 402: return "Оплатите доступ к ресурсу и попробуйте снова!";
            case 403: return "Повторите попытку позже или обратитесь в поддержку!";
            case 404: return "Убедитесь, что указан верный адрес!";
            case 429: return "Попробуйте позже!";

            case 500: return "Повторите попытку позже или обратитесь в поддержку!";
            case 502: return "Повторите попытку позже или обратитесь в поддержку!";
            case 503: return "Повторите попытку позже или обратитесь в поддержку!";
            case 504: return "Повторите попытку позже или обратитесь в поддержку!";
            case 508: return "Повторите попытку позже или обратитесь в поддержку!";

            default: return "Повторите попытку позже или обратитесь в поддержку!";
        }
    }
    app.setNotFoundHandler({ preHandler: app.rateLimit({ max: 25, timeWindow: 60000, ban: 40 }) }, (req, res) => { throw 404; });
    app.setErrorHandler((error, req, res) =>
    {
        if (config.stage == "testing") console.error(error);

        if (Number.isInteger(error)) error = { statusCode: error };
        if (typeof error != "object") error = { statusCode: 500, message: `${error}` };
        error = {
            statusCode: error.statusCode ?? 500,
            title: error.title ?? get_default_error_title(error.statusCode),
            message: error.message ?? get_default_error_message(error.statusCode)
        };
        return res.status(error.statusCode).send(error);
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, {
    name: 'response',
    decorators:
    {
        fastify: [ 'rateLimit' ]
    },
    dependencies: [ '@fastify/rate-limit' ],
    encapsulate: false
});