import config from "common/configs/config.json" assert { type: "json" };

import circuit_breaker from "@fastify/circuit-breaker";
import under_pressure from '@fastify/under-pressure';
import swagger from '@fastify/swagger';

import fs from 'fs';

async function register(app, options)
{
    /*----------------------------------CIRCUIT BREAKER----------------------------------*/
    await app.register(circuit_breaker, {
        threshold: 5, timeout: 10000,
        onCircuitOpen: async (req, res) =>
        {
            console.warn(`@fastify/circuit-breaker: onCircuitOpen triggered for ${req.routerPath}${req.query}${JSON.stringify(req.body)}`);
            throw 508;
        },
        onTimeout: async (req, res) =>
        {
            console.warn(`@fastify/circuit-breaker: onTimeout triggered for ${req.routerPath}${req.query}${JSON.stringify(req.body)}`);
            throw 504;
        }
    });
    



    /*----------------------------------UNDER PRESSURE----------------------------------*/
    await app.register(under_pressure, { 
        maxEventLoopDelay: 250,
        maxHeapUsedBytes: 1048576000,
        maxRssBytes: 1048576000,
        maxEventLoopUtilization: 0.8,
        message: 'The server is exhausted! Try again later!',
        retryAfter: 60,
        pressureHandler: (req, res, type, value) =>
        {
            if (type === under_pressure.TYPE_HEAP_USED_BYTES) { console.warn(`Heap has been exhausted: ${value}`); return; }
            if (type === under_pressure.TYPE_RSS_BYTES) { console.warn(`RSS has been exhausted: ${value}`); return; }
            throw 503;
        },
        exposeStatusRoute:
        {
            routeResponseSchemaOpts:
            {
                metrics:
                {
                    type: 'object',
                    properties: { eventLoopDelay: { type: 'number' }, rssBytes: { type: 'number' }, heapUsed: { type: 'number' }, eventLoopUtilized: { type: 'number' } }
                },
                uptime: { type: 'number' }
            },
            url: "/serverstatus"
        },
        healthCheck: async (fastifyInstance) => ({ metrics: fastifyInstance.memoryUsage(), uptime: process.uptime() }),
    });
    



    /*----------------------------------SWAGGER----------------------------------*/
    await app.register(swagger, {
        openapi:
        {
            servers: [ { url: `https://${config.orders.host}:${config.orders.port}`, description: "Order management server" } ],
            components:
            {
                securitySchemes:
                {
                    cookieAuth: { type: 'apiKey', in: 'cookie', name: "__Secure-Authorization", description: "Default authorization" }
                }
            }
        }
    });

    app.addSchema({ $id: "id", type: "integer", minimum: -2147483648, maximum: 2147483647 });
    app.addSchema({
        $id: "http_error",
        type: "object",
        required: [ "title", "message" ],
        additionalProperties: false,
        properties:
        {
            "title": { type: "string" },
            "message": { type: "string" }
        }
    });
    app.addSchema({
        $id: "http_ok",
        type: "object",
        required: [ "ok" ],
        additionalProperties: false,
        properties:
        {
            "ok": { const: true }
        }
    });
    
    app.addHook("onReady", function (done)
    {
        fs.mkdirSync("documentation", { recursive: true });
        fs.writeFileSync("documentation/orders.yml", app.swagger({ yaml: true }), "utf-8");
        done();
    });
}

import plugin from 'fastify-plugin';
export default plugin(register, { name: 'utility', encapsulate: false });