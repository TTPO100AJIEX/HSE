import { OrdersDatabase } from 'common/postgreSQL/postgreSQL.js';

async function register(app, options)
{
    const UPDATE_DISH_SCHEMA =
    {
        summary: "Update the dish",
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
        body:
        {
            type: "object",
            required: [ "name", "description", "price", "quantity" ],
            additionalProperties: false,
            properties:
            {
                name: { type: "string", maxLength: 100 },
                description: { type: "string" },
                price: { type: "number" },
                quantity: { type: "integer", minimum: 1 }
            }
        },
        security: [ { cookieAuth: [ ] } ],
        response:
        {
            200: { $ref: "http_ok" },
            default: { $ref: "http_error" }
        }
    };
    app.patch("/dish/:id", { schema: UPDATE_DISH_SCHEMA, config: { access: [ 'MANAGER' ] } }, async function(req, res)
    {
        const queryString = `UPDATE dishes SET (name, description, price, quantity) = ($2, $3, $4, $5) WHERE id = $1`;
        const queryParams = [ req.params.id, req.body.name, req.body.description, req.body.price, req.body.quantity ];
        await OrdersDatabase.query(queryString, queryParams);
        return res.send({ ok: true });
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });