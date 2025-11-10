# FILE 5: project/__init__.py
# ============================================
from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_login import LoginManager
from flask_bcrypt import Bcrypt
import os
import sys

parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if parent_dir not in sys.path:
    sys.path.insert(0, parent_dir)

from config import config

db = SQLAlchemy()
bcrypt = Bcrypt()
login_manager = LoginManager()
login_manager.login_view = 'auth.login'
login_manager.login_message_category = 'info'

def create_app(config_name='default'):
    """Application factory function."""
    app = Flask(__name__, instance_relative_config=True)
    
    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass
    
    app.config.from_object(config[config_name])
    config[config_name].init_app(app)

    db.init_app(app)
    bcrypt.init_app(app)
    login_manager.init_app(app)

    from .main import main as main_blueprint
    app.register_blueprint(main_blueprint)

    from .auth import auth as auth_blueprint
    app.register_blueprint(auth_blueprint, url_prefix='/auth')

    from .api import api as api_blueprint
    app.register_blueprint(api_blueprint, url_prefix='/api')

    with app.app_context():
        db.create_all()

    return app