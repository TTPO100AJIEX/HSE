import Chart from "/static/js/utils/chart.js";

var config = undefined, charts = undefined, socket = new WebSocket(`wss://${location.host}/data`);
function requestData()
{
    socket.send(JSON.stringify({ requestName: "database_data" }));
}
socket.addEventListener("message", message =>
{
    const msg = JSON.parse(message.data);
    switch (msg.eventName)
    {
        case "config":
        {
            config = msg.data;
            charts = {
                connections: new Chart( document.getElementById("connections_graph"),
                                        config.graph_records,
                                        config.update_interval,
                                        "Соединения с базой данных",
                                        "Количество соединений",
                                        { active: "Активные", idle: "Бездействующие" }),
                queries: new Chart( document.getElementById("queries_graph"),
                                    config.graph_records,
                                    config.update_interval,
                                    "Запросы к базе данных",
                                    "Количество запросов",
                                    { success: "Успешные", cancelled: "Отменённые" }),
                reads: new Chart(   document.getElementById("reads_graph"),
                                    config.graph_records,
                                    config.update_interval,
                                    "Статистика получения записей",
                                    "Количество записей",
                                    { total: "Всего", index: "По индексу" }),
                updates: new Chart( document.getElementById("updates_graph"),
                                    config.graph_records,
                                    config.update_interval,
                                    "Изменения записей базы данных",
                                    "Количество записей",
                                    { inserted: "Добавленных", updated: "Обновлённых", deleted: "Удалённых" }),
                blocks: new Chart(  document.getElementById("blocks_graph"),
                                    config.graph_records,
                                    config.update_interval,
                                    "Количество обращений к диску",
                                    "Количество обращений",
                                    { total: "Всего", cache: '"Кэш"' }),
                worktime: new Chart(document.getElementById("worktime_graph"),
                                    config.graph_records,
                                    config.update_interval,
                                    "Время исполнения запросов базой данных",
                                    "Время (ms)",
                                    { worktime: "Время исполнения запросов" })
            };
            requestData();
            break;
        }
        case "database_data":
        {
            document.getElementById("database_name").innerText = msg.data.database_name;
            document.getElementById("postgres_version").innerText = msg.data.postgres_version;
            document.getElementById("uptime").innerText = msg.data.uptime;
            document.getElementById("database_size").innerText = msg.data.database_size;
            document.getElementById("stats_reset").innerText = msg.data.reset || "Никогда";
            for (const chartName in charts) charts[chartName].addRecord(msg.data[chartName]);
            setTimeout(requestData, config.update_interval);
            break;
        }
        default:
        {
            console.warn("Websocket - unknown eventName received " + msg.eventName);
        }
    }
}, { "capture": false, "once": false, "passive": true });