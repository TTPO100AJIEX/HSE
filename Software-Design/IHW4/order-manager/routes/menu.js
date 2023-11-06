import { OrdersDatabase } from 'common/postgreSQL/postgreSQL.js';

async function register(app, options)
{
    const GET_MENU_SCHEMA =
    {
        summary: "Get the menu",
        security: [ { cookieAuth: [ ] } ],
        response:
        {
            200:
            {
                type: "array",
                items:
                {
                    type: "object",
                    required: [ "id", "name", "description", "price" ],
                    additionalProperties: false,
                    properties:
                    {
                        id: { $ref: "id" },
                        name: { type: "string", maxLength: 100 },
                        description: { type: "string" },
                        price: { type: "number" }
                    }
                }
            },
            default: { $ref: "http_error" }
        }
    };
    app.get("/menu", { schema: GET_MENU_SCHEMA, config: { access: [ 'VISITOR', 'CHEF', 'MANAGER' ] } }, async function(req, res)
    {
        return res.send(await OrdersDatabase.query(`SELECT id, name, description, price FROM dishes WHERE quantity > 0`));
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });