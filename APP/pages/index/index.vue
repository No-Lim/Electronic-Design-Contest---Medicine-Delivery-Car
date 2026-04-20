<template>
	<view class="container">
		<!-- 标题栏 -->
		<view class="header">
			<text class="header-title">送药车状态监控</text>
		</view>

		<!-- 一号送药车 -->
		<view class="card">
			<view class="card-header">
				<text class="card-title">一号送药车</text>
				<view class="status-indicator" :class="condition1 === '送药中' ? 'active' : 'inactive'"></view>
			</view>
			
			<view class="card-body">
				<image class="device-image" src="../../static/logo.png" mode="aspectFit" />
				<view class="status-container">
					<text class="status-label">状态：</text>
					<text class="status-value" :class="{
						'status-delivering': condition1 === '送药中',
						'status-waiting': condition1 === '等待取药中',
						'status-error': condition1 === '故障'
					}">{{ condition1 }}</text>
				</view>
			</view>
			
			<view class="card-footer">
				<text class="update-time">最后更新: {{ updateTime1 }}</text>
			</view>
		</view>

		<!-- 分隔线 -->
		<view class="divider"></view>

		<!-- 二号送药车 -->
		<view class="card">
			<view class="card-header">
				<text class="card-title">二号送药车</text>
				<view class="status-indicator" :class="condition2 === '送药中' ? 'active' : 'inactive'"></view>
			</view>
			
			<view class="card-body">
				<image class="device-image" src="../../static/logo.png" mode="aspectFit" />
				<view class="status-container">
					<text class="status-label">状态：</text>
					<text class="status-value" :class="{
						'status-delivering': condition2 === '送药中',
						'status-waiting': condition2 === '等待取药中',
						'status-error': condition2 === '故障'
					}">{{ condition2 }}</text>
				</view>
			</view>
			
			<view class="card-footer">
				<text class="update-time">最后更新: {{ updateTime2 }}</text>
			</view>
		</view>

		<!-- 刷新按钮 -->
		<view class="refresh-btn" @click="fetchDevData">
			<text class="refresh-text">刷新状态</text>
			<text class="refresh-icon">↻</text>
		</view>
	</view>
</template>

<script>
	const {
		createCommonToken
	} = require('@/key.js')
	
	export default {
		data() {
			return {
				condition1: '送药中',
				condition2: '等待取药中',
				updateTime1: '--:--:--',
				updateTime2: '--:--:--',
				token: '',
				isLoading: false
			}
		},
		onLoad() {
			this.initToken();
			this.fetchDevData();
			// 定时更新数据（每30秒）
			setInterval(this.fetchDevData, 30000);
		},
		methods: {
			initToken() {
				const params = {
					author_key: '',
					version: '2022-05-01',
					user_id: '',
				}
				this.token = createCommonToken(params);
				console.log('Token:', this.token);
			},
			
			async fetchDevData() {
				if (this.isLoading) return;
				
				this.isLoading = true;
				try {
					// 模拟API请求
					await this.mockApiRequest();
					
					// 实际使用时替换为：
					// const res = await uni.request({
					// 	url: '您的API地址',
					// 	method: 'GET',
					// 	header: {
					// 		'Authorization': `Bearer ${this.token}`
					// 	}
					// });
					
					this.updateTime1 = this.getCurrentTime();
					this.updateTime2 = this.getCurrentTime();
					
					uni.showToast({
						title: '状态更新成功',
						icon: 'success',
						duration: 1500
					});
					
				} catch (error) {
					console.error('获取数据失败:', error);
					uni.showToast({
						title: '更新失败',
						icon: 'error',
						duration: 1500
					});
				} finally {
					this.isLoading = false;
				}
			},
			
			mockApiRequest() {
				return new Promise((resolve) => {
					setTimeout(() => {
						// 模拟随机状态变化
						const statuses = ['送药中', '等待取药中', '故障'];
						this.condition1 = statuses[Math.floor(Math.random() * statuses.length)];
						this.condition2 = statuses[Math.floor(Math.random() * statuses.length)];
						resolve();
					}, 1000);
				});
			},
			
			getCurrentTime() {
				const now = new Date();
				return `${now.getHours().toString().padStart(2, '0')}:${now.getMinutes().toString().padStart(2, '0')}:${now.getSeconds().toString().padStart(2, '0')}`;
			}
		}
	}
</script>

<style>
	.container {
		padding: 20rpx;
		background: linear-gradient(135deg, #f5f7fa 0%, #c3cfe2 100%);
		min-height: 100vh;
	}
	
	.header {
		text-align: center;
		margin-bottom: 40rpx;
		padding: 30rpx 0;
	}
	
	.header-title {
		font-size: 40rpx;
		font-weight: bold;
		color: #2c3e50;
	}
	
	.card {
		background: white;
		border-radius: 20rpx;
		padding: 30rpx;
		margin: 20rpx 0;
		box-shadow: 0 8rpx 25rpx rgba(0,0,0,0.1);
		border-left: 8rpx solid #3498db;
	}
	
	.card-header {
		flex-direction: row;
		justify-content: space-between;
		align-items: center;
		margin-bottom: 30rpx;
	}
	
	.card-title {
		font-size: 36rpx;
		font-weight: 600;
		color: #2c3e50;
	}
	
	.status-indicator {
		width: 20rpx;
		height: 20rpx;
		border-radius: 50%;
	}
	
	.status-indicator.active {
		background-color: #2ecc71;
		box-shadow: 0 0 10rpx #2ecc71;
	}
	
	.status-indicator.inactive {
		background-color: #e74c3c;
	}
	
	.card-body {
		align-items: center;
		margin-bottom: 25rpx;
	}
	
	.device-image {
		width: 300rpx;
		height: 300rpx;
		margin-bottom: 25rpx;
	}
	
	.status-container {
		flex-direction: row;
		align-items: center;
	}
	
	.status-label {
		font-size: 32rpx;
		color: #7f8c8d;
	}
	
	.status-value {
		font-size: 34rpx;
		font-weight: bold;
	}
	
	.status-delivering {
		color: #27ae60;
	}
	
	.status-waiting {
		color: #f39c12;
	}
	
	.status-error {
		color: #e74c3c;
	}
	
	.card-footer {
		border-top: 1rpx solid #ecf0f1;
		padding-top: 20rpx;
	}
	
	.update-time {
		font-size: 28rpx;
		color: #95a5a6;
	}
	
	.divider {
		height: 1rpx;
		background: linear-gradient(90deg, transparent, #bdc3c7, transparent);
		margin: 30rpx 0;
	}
	
	.refresh-btn {
		position: fixed;
		bottom: 50rpx;
		left: 50%;
		transform: translateX(-50%);
		background: linear-gradient(135deg, #3498db, #2980b9);
		padding: 25rpx 50rpx;
		border-radius: 50rpx;
		flex-direction: row;
		align-items: center;
		box-shadow: 0 10rpx 30rpx rgba(52, 152, 219, 0.3);
	}
	
	.refresh-text {
		color: white;
		font-size: 32rpx;
		margin-right: 15rpx;
	}
	
	.refresh-icon {
		color: white;
		font-size: 36rpx;
		font-weight: bold;
	}
	
	/* 响应式设计 */
	@media (max-width: 768px) {
		.card {
			padding: 25rpx;
			margin: 15rpx 0;
		}
		
		.device-image {
			width: 250rpx;
			height: 250rpx;
		}
	}
</style>