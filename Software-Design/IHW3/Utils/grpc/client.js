import util from "util";

import grpc from "@grpc/grpc-js";
import protoLoader from "@grpc/proto-loader";

export default class grpcClientFactory {
	get(protoName, serviceName) {
		const loadOptions = { keepCase: true, includeDirs: ["Utils/grpc/protobufs"], enums: String, defaults: true };
		const BaseClient = grpc.loadPackageDefinition(protoLoader.loadSync(protoName, loadOptions))[serviceName];
		return class grpcClient extends BaseClient {
			constructor(port) {
				super(`localhost:${port}`, grpc.credentials.createInsecure());
				for (const key in this) this[key] = util.promisify(this[key]);
			}
		};
	}

	static get(...args) {
		return new grpcClientFactory().get(...args);
	}
}

const ManagerClient = grpcClientFactory.get("manager.proto", "manager");
const StewardClient = grpcClientFactory.get("steward.proto", "steward");
const StorageClient = grpcClientFactory.get("storage.proto", "storage");
const KitchenClient = grpcClientFactory.get("kitchen.proto", "kitchen");
const CookClient = grpcClientFactory.get("cook.proto", "cook");
const EquipmentClient = grpcClientFactory.get("equipment.proto", "equipment");
export { ManagerClient, StewardClient, StorageClient, KitchenClient, CookClient, EquipmentClient };
