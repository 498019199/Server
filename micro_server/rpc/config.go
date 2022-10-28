package rpc

import "micro_serverr/core/discov"

type (
	ServerConfig struct {
	}

	RpcServerConf struct {
		extension_name string `yaml: "extension_name" ,default=extesion_name`
		address        string `yaml: "address, default="127.0.0.1:8090`
		consul         discov.ConsulConf
		log_path       string `yaml: "log_path"`
		log_prefix     string `yaml: "log_prefix"`
		log_level      int    `yaml: "log_level", default=4`
		health         bool   `yaml: "health", default=true`
	}

	RpcClientConf struct {
		address []string `yaml: "address" ,default="127.0.0.1:8090"`
		Timeout int64    `yaml: "timeout", default=2000`
	}
)

// HasEtcd checks if there is etcd settings in config.
func (sc RpcServerConf) HasConsul() bool {
	return len(sc.consul.Host) > 0 && len(sc.consul.Key) > 0
}
