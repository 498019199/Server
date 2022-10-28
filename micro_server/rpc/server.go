package rpc

import (
	"google.golang.org/grpc"
	"log"
	"micro_serverr/core/stat"
	"micro_serverr/core/xlog"
	"micro_serverr/rpc/internal"
	"time"
)

// A RpcServer is a rpc server.
type RpcServer struct {
	server   internal.Server
	register internal.RegisterFn
}

// MustNewServer returns a RpcSever, exits on any error.
func MustNewServer(c RpcServerConf, register internal.RegisterFn) *RpcServer {
	server, err := NewServer(c, register)
	if err != nil {
		log.Fatal(err)
	}

	return server
}

// NewServer returns a RpcServer.
func NewServer(c RpcServerConf, register internal.RegisterFn) (*RpcServer, error) {
	var err error

	var server internal.Server
	metrics := stat.NewMetrics(c.address)
	serverOptions := []internal.ServerOption{
		internal.WithMetrics(metrics),
		internal.WithRpcHealth(c.health),
	}

	if c.HasConsul() {
		server, err = internal.NewRpcPubServer(c.consul, c.address, serverOptions...)
		if err != nil {
			return nil, err
		}
	} else {
		server = internal.NewRpcServer(c.address, serverOptions...)
	}

	server.SetName(c.extension_name)
	if err = setupInterceptors(server, c, metrics); err != nil {
		return nil, err
	}

	rpcServer := &RpcServer{
		server:   server,
		register: register,
	}
	//if err = c.SetUp(); err != nil {
	//	return nil, err
	//}

	return rpcServer, nil
}

// AddOptions adds given options.
func (rs *RpcServer) AddOptions(options ...grpc.ServerOption) {
	rs.server.AddOptions(options...)
}

// AddStreamInterceptors adds given stream interceptors.
func (rs *RpcServer) AddStreamInterceptors(interceptors ...grpc.StreamServerInterceptor) {
	rs.server.AddStreamInterceptors(interceptors...)
}

// AddUnaryInterceptors adds given unary interceptors.
func (rs *RpcServer) AddUnaryInterceptors(interceptors ...grpc.UnaryServerInterceptor) {
	rs.server.AddUnaryInterceptors(interceptors...)
}

// Start starts the RpcServer.
// Graceful shutdown is enabled by default.
// Use proc.SetTimeToForceQuit to customize the graceful shutdown period.
func (rs *RpcServer) Start() {
	if err := rs.server.Start(rs.register); err != nil {
		xlog.Error(err)
		panic(err)
	}
}

// Stop stops the RpcServer.
func (rs *RpcServer) Stop() {
	xlog.Close()
}

// DontLogContentForMethod disable logging content for given method.
func DontLogContentForMethod(method string) {
	//serverinterceptors.DontLogContentForMethod(method)
}

// SetServerSlowThreshold sets the slow threshold on server side.
func SetServerSlowThreshold(threshold time.Duration) {
	//serverinterceptors.SetSlowThreshold(threshold)
}

func setupInterceptors(server internal.Server, c RpcServerConf, metrics *stat.Metrics) error {
	//if c.CpuThreshold > 0 {
	//	shedder := load.NewAdaptiveShedder(load.WithCpuThreshold(c.CpuThreshold))
	//	server.AddUnaryInterceptors(serverinterceptors.UnarySheddingInterceptor(shedder, metrics))
	//}
	//
	//if c.Timeout > 0 {
	//	server.AddUnaryInterceptors(serverinterceptors.UnaryTimeoutInterceptor(
	//		time.Duration(c.Timeout) * time.Millisecond))
	//}
	//
	//if c.Auth {
	//	authenticator, err := auth.NewAuthenticator(c.Redis.NewRedis(), c.Redis.Key, c.StrictControl)
	//	if err != nil {
	//		return err
	//	}
	//
	//	server.AddStreamInterceptors(serverinterceptors.StreamAuthorizeInterceptor(authenticator))
	//	server.AddUnaryInterceptors(serverinterceptors.UnaryAuthorizeInterceptor(authenticator))
	//}

	return nil
}
