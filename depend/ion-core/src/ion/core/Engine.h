/*
 * Copyright 2023 Markus Haikonen, Ionhaken
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

namespace ion
{
class Engine
{
	friend class GTestHelper;
public:
	Engine();
	~Engine();

	static bool IsActive();

	static bool IsExitRequested();

	static bool IsDynamicInitExit();

	static bool IsDynamicInitDone();

	static void RequestExit();

	static void InstallHandlers();

	static void ClearHandlers();


private:
	static void Restart();

	static void Start();

	static void Stop();

	static void StartInternal();
};
}  // namespace ion
