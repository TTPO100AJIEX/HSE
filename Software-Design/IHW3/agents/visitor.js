import { ManagerClient } from "utils/grpc/client.js";
import sleep from "utils/sleep.js";

class Visitor {
	#manager;
	#orderId;
	constructor(managerPort) {
		this.#manager = new ManagerClient(managerPort);
		this.#run();
	}
	async #run() {
		const menu = await this.#manager.getMenu({});
		if (!menu.dishes) return console.log("Menu is empty");
		const chosen = await this.#choose(menu);
		const order = await this.#manager.placeOrder(chosen);
		if (order.ok) {
			this.#orderId = order.orderId;
			this.#scheduleStatusCheck();
		} else {
			console.log(order.error);
		}
	}

	async #choose(menu) {
		const MAX_SLEEP = 10000,
			MIN_SLEEP = 500;
		await sleep(Math.random() * (MAX_SLEEP - MIN_SLEEP) + MIN_SLEEP);
		const orderItems = Math.floor(Math.random() * 5) + 1; // Order 1 to 5 dishes
		return { dishIds: new Array(orderItems).fill(undefined).map(_ => menu.dishes[Math.floor(Math.random() * menu.dishes.length)].id) };
	}

	async #scheduleStatusCheck() {
		const MAX_INTERVAL = 10000,
			MIN_INTERVAL = 500;
		setTimeout(this.#askOrderStatus.bind(this), Math.random() * (MAX_INTERVAL - MIN_INTERVAL) + MIN_INTERVAL);
	}
	async #askOrderStatus() {
		const { status, waitTime } = await this.#manager.getOrderStatus({ id: this.#orderId });
		console.log(status);
		if (status == "COOKING") console.log(waitTime);
		if (status == "READY") {
			const { ok, food } = await this.#manager.getOrder({ id: this.#orderId });
			if (ok) return this.#eat(food);
		}
		this.#scheduleStatusCheck();
	}

	async #eat(food) {
		console.log("Eating " + food.join(", "));
		const MAX_SLEEP = 15000,
			MIN_SLEEP = 2000;
		await sleep(Math.random() * (MAX_SLEEP - MIN_SLEEP) + MIN_SLEEP);
	}
}

const visitor = new Visitor(process.argv[2]);
