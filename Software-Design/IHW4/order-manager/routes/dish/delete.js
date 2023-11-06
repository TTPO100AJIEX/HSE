import { OrdersDatabase } from 'common/postgreSQL/postgreSQL.js';

async function register(app, options)
{
    const DELETE_DISH_SCHEMA =
    {
        summary: "Delete the dish",
        params:
        {
            type: "object",
            required: [ "id" ],
            additionalProperties: false,
            properties:
            {
                "id": { $ref: "id" }
            }
        },
        security: [ { cookieAuth: [ ] } ],
        response:
        {
            200: { $ref: "http_ok" },
            default: { $ref: "http_error" }
        }
    };
    app.delete("/dish/:id", { schema: DELETE_DISH_SCHEMA, config: { access: [ 'MANAGER' ] } }, async function(req, res)
    {
        await OrdersDatabase.query(`DELETE FROM dishes WHERE id = $1`, [ req.params.id ]);
        return res.send({ ok: true });
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });