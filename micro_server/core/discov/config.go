package discov

// EtcdConf is the config item with the given key on etcd.
type ConsulConf struct {
	Host     string
	ListenOn string
	Key      string
	Token    string            `json:",optional"`
	Tag      []string          `json:",optional"`
	Meta     map[string]string `json:",optional"`
	TTL      int               `json:"ttl,optional"`
}
