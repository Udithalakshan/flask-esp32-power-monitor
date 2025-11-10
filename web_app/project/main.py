# project/main.py
# ============================================
from flask import Blueprint, render_template, redirect, url_for
from flask_login import login_required, current_user

main = Blueprint('main', __name__)

@main.route('/')
@main.route('/index')
def index():
    """Home page - redirects to dashboard if logged in"""
    if current_user.is_authenticated:
        return redirect(url_for('main.dashboard'))
    return redirect(url_for('auth.login'))

@main.route('/dashboard')
@login_required
def dashboard():
    """Dashboard page - requires login"""
    return render_template('dashboard.html', title='Power Analyzer Dashboard', user=current_user)