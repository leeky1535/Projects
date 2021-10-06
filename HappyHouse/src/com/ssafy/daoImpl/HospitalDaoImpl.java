package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.HospitalDao;
import com.ssafy.dto.HospitalDto;
import com.ssafy.util.DBClose;
import com.ssafy.util.DBConnection;

public class HospitalDaoImpl implements HospitalDao {

	private final String DRIVER = "com.mysql.cj.jdbc.Driver";

	private static HospitalDao hospitalDao;

	private HospitalDaoImpl() {
		try {
			Class.forName(DRIVER);
			System.out.println("드라이버 로딩 성공");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println("드라이버 로딩 실패");
		}
	}

	public static HospitalDao getHospitalDao() {
		if (hospitalDao == null)
			hospitalDao = new HospitalDaoImpl();
		return hospitalDao;
	}

	@Override
	public List<HospitalDto> searchByGugun(String gugun) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<HospitalDto> hospitalDtoList = new ArrayList<HospitalDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select date, gugun_code, name, address, type, phone " + "from hospital "
					+ "where gugun_code = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, gugun);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				HospitalDto hospitalDto = new HospitalDto(rs.getString("date"), rs.getString("gugun_code"),
						rs.getString("name"), rs.getString("address"), rs.getString("type"), rs.getString("phone"));
				hospitalDtoList.add(hospitalDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return hospitalDtoList;
	}

	@Override
	public List<HospitalDto> searchAll() {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<HospitalDto> hospitalDtoList = new ArrayList<HospitalDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select date_effective, gugun_code, name, address, business_hours_holiday, phone "
					+ "from houseinfo";
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				HospitalDto hospitalDto = new HospitalDto(rs.getString("date"), rs.getString("gugun_code"),
						rs.getString("name"), rs.getString("address"), rs.getString("type"), rs.getString("phone"));
				hospitalDtoList.add(hospitalDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return hospitalDtoList;
	}

}
