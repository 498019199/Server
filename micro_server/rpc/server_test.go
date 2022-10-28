package rpc

import (
	"testing"
	"time"

	"google.golang.org/grpc"

	"micro_serverr/core/discov"
	"micro_serverr/rpc/internal/serverinterceptors"
)

// 测试consul注册
func TestServer_HasConsul(t *testing.T) {

	svr := MustNewServer(RpcServerConf{
		extension_name: "extension_name",
		address:        "127.0.0.1:12333",
		consul: discov.ConsulConf{
			Host: "127.0.0.1:8500",
			Key:  "test_server",
			//Tag:  ["test"],
		},
		log_path:   "D:\\Test\\GitHub\\Server\\Server\\tmp\\log",
		log_prefix: "test_server",
		log_level:  4,
		health:     false,
	}, func(server *grpc.Server) {
		//
	})

	svr.AddOptions(grpc.ConnectionTimeout(time.Hour))
	svr.AddUnaryInterceptors(serverinterceptors.UnaryCrashInterceptor)
	svr.AddStreamInterceptors(serverinterceptors.StreamCrashInterceptor)
	go svr.Start()
	svr.Stop()
}
