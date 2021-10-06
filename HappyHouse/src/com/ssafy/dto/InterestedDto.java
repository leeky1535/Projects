package com.ssafy.dto;

public class InterestedDto {
	private String userId;
	private String dongCode;

	public InterestedDto(String userId, String dongCode) {
		super();
		this.userId = userId;
		this.dongCode = dongCode;
	}

	public String getUserId() {
		return userId;
	}

	public void setUserId(String userId) {
		this.userId = userId;
	}

	public String getDongCode() {
		return dongCode;
	}

	public void setDongCode(String dongCode) {
		this.dongCode = dongCode;
	}

	@Override
	public String toString() {
		return "InterestedDto [userId=" + userId + ", dongCode=" + dongCode + "]";
	}

}
