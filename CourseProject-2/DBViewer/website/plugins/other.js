import fastify_plugin from 'fastify-plugin';

import config from 'common/configs/config.js';

import fastify_cookie from "@fastify/cookie";

function register_other_plugins(app, options, done)
{
    /*----------------------------------COOKIE----------------------------------*/
    app.register(fastify_cookie, { secret: config.website.secret });
    
    done();
}

export default fastify_plugin(register_other_plugins, { name: 'other', encapsulate: false });