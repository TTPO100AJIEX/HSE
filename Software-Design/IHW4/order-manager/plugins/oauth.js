import config from "common/configs/config.json" assert { type: "json" };

async function register(app, options)
{
    app.decorateRequest("authorization", null);
    app.decorateRequest("authorize", async function()
    {
        if (this.authorization) return this.authorization;
        const response = await fetch(`http://${config.accounts.host}:${config.accounts.port}`, {
            method: "GET",
            headers: { cookie: this.headers.cookie }
        });
        return this.authorization = await response.json();
    });

    app.addHook('preHandler', async (req, res) => 
    {
        if (!req.routeConfig?.access) return;
        const authorization = await req.authorize();
        if (!req.routeConfig.access.includes(authorization?.role || null)) throw 403;
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'oauth', encapsulate: false });