import { OrdersDatabase } from 'common/postgreSQL/postgreSQL.js';

async function register(app, options)
{
    const GET_ORDER_SCHEMA =
    {
        summary: "Get information about the order",
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
            200:
            {
                id: { $ref: "id" },
                user_id: { $ref: "id" },
                status: { enum: [ 'WAITING', 'PROCESSING', 'COMPLETED', 'CANCELLED' ] },
                special_requests: { type: "string" },
                created_at: { type: "string", format: "iso-date-time" },
                updated_at: { type: "string", format: "iso-date-time" },
                dishes:
                {
                    type: "array",
                    items:
                    {
                        type: "object",
                        required: [ "id", "name", "price", "quantity", "description" ],
                        additionalProperties: false,
                        properties:
                        {
                            id: { $ref: "id" },
                            name: { type: "string", maxLength: 100 },
                            price: { type: "number" },
                            quantity: { type: "integer", minimum: 1 },
                            description: { type: "string" }
                        }
                    }
                }
            },
            default: { $ref: "http_error" }
        }
    };
    app.get("/order/:id", { schema: GET_ORDER_SCHEMA, config: { access: [ 'CUSTOMER', 'CHEF', 'MANAGER' ] } }, async function(req, res)
    {
        const dish_to_json = `to_jsonb(dishes.*) || (to_jsonb(order_dishes.*) - 'dish_id' - 'order_id')`;
        const dishesQueryString = `SELECT array_agg(${dish_to_json}) FROM order_dishes INNER JOIN dishes ON order_dishes.dish_id = dishes.id WHERE order_id = orders.id`;
        const queryString = `SELECT id, user_id, status, special_requests, created_at, updated_at, (${dishesQueryString}) AS dishes FROM orders WHERE id = $1`;
        const orderData = await OrdersDatabase.query(queryString, [ req.params.id ], { one_response: true });
        if (!orderData) throw { statusCode: 404, message: `Order with id ${req.params.id} does not exist` };
        return res.send(orderData);
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'routes', encapsulate: false });