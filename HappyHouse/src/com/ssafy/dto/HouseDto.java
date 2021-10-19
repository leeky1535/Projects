package com.ssafy.dto;

public class HouseDto implements Comparable<HouseDto> {
	private int no;
	private String sido;
	private String gugun;
	private String dong;
	private String dongCode;
	private String aptName;
	private String buildYear;
	private String jibun;
	private String lat;
	private String lng;
	private String img;

	public HouseDto(int no, String sido, String gugun, String dong, String dongCode, String aptName, String buildYear,
			String jibun, String lat, String lng, String img) {
		super();
		this.no = no;
		this.sido = sido;
		this.gugun = gugun;
		this.dong = dong;
		this.dongCode = dongCode;
		this.aptName = aptName;
		this.buildYear = buildYear;
		this.jibun = jibun;
		this.lat = lat;
		this.lng = lng;
		this.img = img;
	}

	public String getImg() {
		return img;
	}

	public void setImg(String img) {
		this.img = img;
	}

	public int getNo() {
		return no;
	}

	public String getSido() {
		return sido;
	}

	public void setSido(String sido) {
		this.sido = sido;
	}

	public String getGugun() {
		return gugun;
	}

	public void setGugun(String gugun) {
		this.gugun = gugun;
	}

	public void setNo(int no) {
		this.no = no;
	}

	public String getDong() {
		return dong;
	}

	public void setDong(String dong) {
		this.dong = dong;
	}

	public String getDongCode() {
		return dongCode;
	}

	public void setDongCode(String dongCode) {
		this.dongCode = dongCode;
	}

	public String getAptName() {
		return aptName;
	}

	public void setAptName(String aptName) {
		this.aptName = aptName;
	}

	public String getBuildYear() {
		return buildYear;
	}

	public void setBuildYear(String buildYear) {
		this.buildYear = buildYear;
	}

	public String getJibun() {
		return jibun;
	}

	public void setJibun(String jibun) {
		this.jibun = jibun;
	}

	public String getLat() {
		return lat;
	}

	public void setLat(String lat) {
		this.lat = lat;
	}

	public String getLng() {
		return lng;
	}

	public void setLng(String lng) {
		this.lng = lng;
	}

	@Override
	public int compareTo(HouseDto o) {
		// TODO Auto-generated method stub
		return this.aptName.compareTo(o.getAptName());
	}

}