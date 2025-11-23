import json

class Config:
    _instance = None
    
    def __new__(cls):
        if cls._instance is None:
            cls._instance = super(Config, cls).__new__(cls)
            cls._instance.load_config()
        return cls._instance
    
    def load_config(self, config_file="config.json"):
        with open(config_file, 'r') as f:
            self.data = json.load(f)
    
    def __getattr__(self, name):
        if name in self.data:
            return self.data[name]
        raise AttributeError(f"Config has no attribute '{name}'")



