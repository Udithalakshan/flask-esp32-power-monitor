import os

basedir = os.path.abspath(os.path.dirname(__file__))

class Config:
    """Base configuration class."""
    SECRET_KEY = os.environ.get('SECRET_KEY') or 'you-will-never-guess-change-in-production'
    SQLALCHEMY_TRACK_MODIFICATIONS = False
    
    # CRITICAL FIX: Removed insecure default fallback. 
    # Must rely entirely on the environment variable loaded by dotenv.
    ESP32_API_KEY = os.environ.get('ESP32_API_KEY') 

    @staticmethod
    def init_app(app):
        pass

class DevelopmentConfig(Config):
    """Development configuration."""
    DEBUG = True
    # SECONDARY FIX: Ensure the config uses DATABASE_URL defined in the.env file
    SQLALCHEMY_DATABASE_URI = os.environ.get('DATABASE_URL') or \
        'sqlite:///' + os.path.join(basedir, 'instance', 'power_analyzer.db')

class ProductionConfig(Config):
    """Production configuration."""
    SQLALCHEMY_DATABASE_URI = os.environ.get('DATABASE_URL') or \
        'sqlite:///' + os.path.join(basedir, 'instance', 'power_analyzer.db')

config = {
    'development': DevelopmentConfig,
    'production': ProductionConfig,
    'default': DevelopmentConfig
}