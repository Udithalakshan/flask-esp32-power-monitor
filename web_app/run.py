import os
import sys
from dotenv import load_dotenv

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from project import create_app, db
from flask_migrate import Migrate

load_dotenv()

app = create_app(os.getenv('FLASK_CONFIG') or 'development')
migrate = Migrate(app, db)

if __name__ == '__main__':
    with app.app_context():
        db.create_all()
    
    # IMPORTANT: Listen on all interfaces (0.0.0.0) so ESP32 can connect
    print("\n" + "="*60)
    print("🚀 Flask Server Starting...")
    print("="*60)
    print(f"📍 Server will be accessible at:")
    print(f"   - Local: http://127.0.0.1:5000")
    print(f"   - Network: http://<YOUR_IP>:5000")
    print(f"   - ESP32 should connect to: http://<YOUR_IP>:5000/api/data")
    print("="*60 + "\n")
    
    app.run(
        host='0.0.0.0',  # Listen on all interfaces
        port=5000,
        debug=True,
        use_reloader=False  # Prevents double initialization
    )