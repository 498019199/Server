package internal

import (
	"fmt"
	"log"
	"net"
	"os"
	"strconv"
	"strings"
	"time"

	consulapi "github.com/hashicorp/consul/api"

	"micro_serverr/core/discov"
	"micro_serverr/core/xlog"
	"micro_serverr/core/xnet"
)

// NewRpcPubServer returns a Server.
func NewRpcPubServer(c discov.ConsulConf, listenOn string, opts ...ServerOption) (Server, error) {
	Server := NewRpcServer(listenOn, opts...)
	pubListenOn := figureOutListenOn(listenOn)
	host, ports, err := net.SplitHostPort(pubListenOn)
	if err != nil {
		return Server, fmt.Errorf("failed parsing address error: %v", err)
	}
	port, _ := strconv.ParseUint(ports, 10, 16)
	// 服务节点的名称
	serviceID := fmt.Sprintf("%s-%s-%d", c.Key, host, port)
	if c.TTL <= 0 {
		c.TTL = 20
	}
	ttl := fmt.Sprintf("%ds", c.TTL)
	expiredTTL := fmt.Sprintf("%ds", c.TTL*3)

	// 创建连接consul服务配置
	config := consulapi.DefaultConfig()
	config.Address = c.Host
	client, err := consulapi.NewClient(config)
	if err != nil {
		log.Fatal("consul client error : ", err)
	}
	// 创建注册到consul的服务到
	reg := &consulapi.AgentServiceRegistration{
		ID:      serviceID, // 服务节点的名称
		Name:    c.Key,     // 服务名称
		Tags:    c.Tag,     // tag，可以为空
		Meta:    c.Meta,    // meta， 可以为空
		Port:    int(port), // 服务端口
		Address: host,      // 服务 IP
		Checks: []*consulapi.AgentServiceCheck{ // 健康检查
			{
				CheckID:                        serviceID, // 服务节点的名称
				TTL:                            ttl,       // 健康检查间隔
				Status:                         "passing",
				DeregisterCriticalServiceAfter: expiredTTL, // 注销时间，相当于过期时间
			},
		},
	}
	err = client.Agent().ServiceRegister(reg)
	if nil != err {
		return Server, fmt.Errorf("initial register service '%s' host to consul error: %s", c.Key, err.Error())
	}

	// initial register service check
	check := consulapi.AgentServiceCheck{
		TTL:                            ttl,
		Status:                         "passing",
		DeregisterCriticalServiceAfter: expiredTTL}
	err = client.Agent().CheckRegister(&consulapi.AgentCheckRegistration{ID: serviceID, Name: c.Key, ServiceID: serviceID, AgentServiceCheck: check})
	if err != nil {
		return Server, fmt.Errorf("initial register service check to consul error: %s", err.Error())
	}

	ttlTicker := time.Duration(c.TTL-1) * time.Second
	if ttlTicker < time.Second {
		ttlTicker = time.Second
	}

	// routine to update ttl
	go func() {
		ticker := time.NewTicker(ttlTicker)
		defer ticker.Stop()
		for {
			<-ticker.C
			err = client.Agent().UpdateTTL(serviceID, "", "passing")
			xlog.Info("update ttl")
			if err != nil {
				xlog.Infof("update ttl of service error: %v", err.Error())
			}
		}
	}()

	// consul deregister
	//proc.AddShutdownListener(func() {
	//	err := client.Agent().ServiceDeregister(serviceID)
	//	if err != nil {
	//		xlog.Info("deregister service error: ", err.Error())
	//	}
	//	xlog.Info("deregistered service from consul server.")
	//})
	return Server, nil
}

func figureOutListenOn(listenOn string) string {
	fields := strings.Split(listenOn, ":")
	if len(fields) == 0 {
		return listenOn
	}

	host := fields[0]
	if len(host) > 0 && host != "0.0.0.0" {
		return listenOn
	}

	ip := os.Getenv("POD_IP")
	if len(ip) == 0 {
		ip = xnet.InternalIp()
	}
	if len(ip) == 0 {
		return listenOn
	}

	return strings.Join(append([]string{ip}, fields[1:]...), ":")
}
