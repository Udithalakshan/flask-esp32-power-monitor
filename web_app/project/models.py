# project/models.py
from . import db, login_manager
from flask_login import UserMixin
from datetime import datetime
import pytz

@login_manager.user_loader
def load_user(user_id):
    return User.query.get(int(user_id))

class User(UserMixin, db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(150), unique=True, nullable=False)
    email = db.Column(db.String(150), unique=True, nullable=False)
    password = db.Column(db.String(256), nullable=False)
    readings = db.relationship('PowerReading', backref='author', lazy=True)

    def __repr__(self):
        return f'<User {self.username}>'

class PowerReading(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    timestamp = db.Column(db.DateTime, nullable=False, 
                         default=lambda: datetime.now(pytz.timezone('Asia/Colombo')).replace(tzinfo=None))
    
    # Phase Voltages (L-N)
    voltage_a = db.Column(db.Float, nullable=True)
    voltage_b = db.Column(db.Float, nullable=True)
    voltage_c = db.Column(db.Float, nullable=True)
    
    # Line Voltages (L-L)
    voltage_ab = db.Column(db.Float, nullable=True)
    voltage_bc = db.Column(db.Float, nullable=True)
    voltage_ca = db.Column(db.Float, nullable=True)
    
    # Phase Currents
    current_a = db.Column(db.Float, nullable=True)
    current_b = db.Column(db.Float, nullable=True)
    current_c = db.Column(db.Float, nullable=True)
    
    # Power Measurements
    power_factor = db.Column(db.Float, nullable=True)
    frequency = db.Column(db.Float, nullable=True)
    active_power = db.Column(db.Float, nullable=True)
    reactive_power = db.Column(db.Float, nullable=True)
    apparent_power = db.Column(db.Float, nullable=True)
    
    # THD Measurements
    voltage_thd_avg = db.Column(db.Float, nullable=True)
    current_thd_avg = db.Column(db.Float, nullable=True)
    
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'), nullable=False)

    def __repr__(self):
        return f'<PowerReading {self.id} at {self.timestamp}>'
    
    def to_dict(self):
        """Convert reading to dictionary for API response"""
        return {
            'id': self.id,
            'timestamp': self.timestamp.strftime('%Y-%m-%d %H:%M:%S'),
            'voltage_a': self.voltage_a,
            'voltage_b': self.voltage_b,
            'voltage_c': self.voltage_c,
            'voltage_ab': self.voltage_ab,
            'voltage_bc': self.voltage_bc,
            'voltage_ca': self.voltage_ca,
            'current_a': self.current_a,
            'current_b': self.current_b,
            'current_c': self.current_c,
            'power_factor': self.power_factor,
            'frequency': self.frequency,
            'active_power': self.active_power,
            'reactive_power': self.reactive_power,
            'apparent_power': self.apparent_power,
            'voltage_thd_avg': self.voltage_thd_avg,
            'current_thd_avg': self.current_thd_avg
        }