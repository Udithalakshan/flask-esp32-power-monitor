# project/api.py
# ===========================================================
from flask import Blueprint, request, jsonify, current_app
from .models import User, PowerReading
from . import db
from flask_login import login_required
from functools import wraps

api = Blueprint('api', __name__)

# -----------------------------------------------------------
# API KEY AUTHENTICATION
# -----------------------------------------------------------
def require_apikey(view_function):
    @wraps(view_function)
    def decorated_function(*args, **kwargs):
        expected_key = current_app.config.get('ESP32_API_KEY')
        provided_key = request.headers.get('X-API-Key')

        if provided_key == 'yPowerAnalyzerKey456':
            return view_function(*args, **kwargs)
        else:
            print(f"[API SECURITY] ❌ Unauthorized - Key: {provided_key}")
            return jsonify({
                "status": "error",
                "message": "Unauthorized - Invalid or missing API key"
            }), 401
    return decorated_function

# -----------------------------------------------------------
# ESP32 DATA UPLOAD ENDPOINT
# -----------------------------------------------------------
@api.route('/data', methods=['POST'])
@require_apikey
def add_data():
    """Receives Modbus readings from ESP32"""
    try:
        data = request.get_json(silent=True)
        if not data:
            return jsonify({"status": "error", "message": "No JSON data received"}), 400

        # Get first registered user (for association)
        user = User.query.first()
        if not user:
            return jsonify({
                "status": "error",
                "message": "No user found. Please register via /auth/register first."
            }), 500

        # Create a PowerReading record
        reading = PowerReading(
            voltage_a=data.get('voltage_a'),
            voltage_b=data.get('voltage_b'),
            voltage_c=data.get('voltage_c'),
            voltage_ab=data.get('voltage_ab'),
            voltage_bc=data.get('voltage_bc'),
            voltage_ca=data.get('voltage_ca'),
            current_a=data.get('current_a'),
            current_b=data.get('current_b'),
            current_c=data.get('current_c'),
            power_factor=data.get('power_factor'),
            frequency=data.get('frequency'),
            active_power=data.get('active_power'),
            reactive_power=data.get('reactive_power'),
            apparent_power=data.get('apparent_power'),
            voltage_thd_avg=data.get('voltage_thd_avg'),
            current_thd_avg=data.get('current_thd_avg'),
            author=user
        )

        db.session.add(reading)
        db.session.commit()

        print(f"[API DATA] ✅ New record saved (ID={reading.id})")
        return jsonify({
            "status": "success",
            "message": "Data received and saved successfully",
            "data": reading.to_dict()
        }), 201

    except Exception as e:
        db.session.rollback()
        print(f"[API ERROR] {e}")
        return jsonify({"status": "error", "message": str(e)}), 500

# -----------------------------------------------------------
# FETCH ALL READINGS (Dashboard)
# -----------------------------------------------------------
@api.route('/data', methods=['GET'])
@login_required
def get_data():
    try:
        readings = PowerReading.query.order_by(PowerReading.timestamp.desc()).all()
        return jsonify({
            "status": "success",
            "count": len(readings),
            "data": [r.to_dict() for r in readings]
        })
    except Exception as e:
        print(f"[API ERROR] {e}")
        return jsonify({"status": "error", "message": str(e)}), 500

# -----------------------------------------------------------
# FETCH LATEST READING
# -----------------------------------------------------------
@api.route('/latest', methods=['GET'])
@login_required
def get_latest():
    try:
        latest = PowerReading.query.order_by(PowerReading.timestamp.desc()).first()
        if latest:
            return jsonify({
                "status": "success",
                "data": latest.to_dict()
            })
        return jsonify({
            "status": "success",
            "data": None,
            "message": "No readings available"
        })
    except Exception as e:
        print(f"[API ERROR] {e}")
        return jsonify({"status": "error", "message": str(e)}), 500

# -----------------------------------------------------------
# STATUS ENDPOINT
# -----------------------------------------------------------
@api.route('/status', methods=['GET'])
def api_status():
    """Health check endpoint"""
    return jsonify({
        "status": "online",
        "message": "Power Analyzer API is running",
        "endpoints": {
            "POST /api/data": "Submit power readings (requires API key)",
            "GET /api/data": "Get all readings (requires login)",
            "GET /api/latest": "Get latest reading (requires login)"
        }
    })