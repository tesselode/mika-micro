local startAngle = 135
local endAngle = 405
local frames = 100

local function drawKnob(originAngle, angle)
	love.graphics.push 'all'

	love.graphics.setLineWidth(6)

	-- body
	love.graphics.setColor(230, 230, 230)
	love.graphics.circle('fill', 32, 32, 22, 64)

	-- outer edge
	love.graphics.setBlendMode('alpha', 'premultiplied')
	love.graphics.setColor(128, 128, 128)
	love.graphics.circle('line', 32, 32, 22, 64)

	-- inner circle
	love.graphics.setBlendMode('alpha', 'alphamultiply')
	love.graphics.setColor(189, 21, 113)
	love.graphics.circle('fill',
		32 + 10 * math.cos(math.rad(angle)),
		32 + 10 * math.sin(math.rad(angle)),
		3.5, 64)

	-- edge accent
	if math.abs(originAngle - angle) > 2 then
		love.graphics.arc('line', 'open', 32, 32, 22,
			math.rad(originAngle), math.rad(angle), 64)
	end

	love.graphics.pop()
end

local function renderKnobStrip(originAngle, filename)
	local canvas = love.graphics.newCanvas(64, 64 * frames,
		'normal', love.graphics.getSystemLimits().canvasmsaa)
	canvas:renderTo(function()
		for i = 1, frames do
			local angle = startAngle + (endAngle - startAngle) * ((i - 1) / (frames - 1))
			love.graphics.push()
			love.graphics.translate(0, 64 * (i - 1))
			drawKnob(originAngle, angle)
			love.graphics.pop()
		end
	end)
	canvas:newImageData():encode('png', filename)
end

renderKnobStrip(startAngle, 'knob left.png')
renderKnobStrip((startAngle + endAngle) / 2, 'knob middle.png')
renderKnobStrip(endAngle, 'knob right.png')